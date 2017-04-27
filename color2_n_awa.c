#include <string.h>
#include <stdio.h>
#include "contiki.h"
#include "contiki-net.h"
#include <awa/static.h>
#include <letmecreate/letmecreate.h>
#include "letmecreate/core/debug.h"
#include <time.h>

PROCESS(ewdemo_process, "EW Demo");
AUTOSTART_PROCESSES(&ewdemo_process);

#define IPSO_COLOUR_OBJECT 3335
#define IPSO_COLOUR 5706

#ifndef IDENTITY
#error Please set the IDENTITY
#endif

#ifndef SECRET_KEY
#error Please set the SECRET_KEY
#endif

#ifndef BINARY_KEY
#error Please set the BINARY_KEY
#endif

static const unsigned char binaryKey[] = {BINARY_KEY};

//---
#define LED1_PORT   B
#define LED1_PIN    1
#define LED2_PORT   B
#define LED2_PIN    2
//----
static char currentColour[250];

//0-none(belt/red), 1-yellow, 2-green, 3-blue, 4-orange, 5-Purple
static int8_t lastColour = 0;
static int waitTime = 1000;

bool nearColorComponent(float value, float expected) {
  static const float diff = 10;
  return (value >= (expected - diff)) && (value <= (expected + diff));
}

bool nearLumosityComponent(float value, float expected) {
  static const float diff = 0.03f;
  return (value >= (expected - diff)) && (value <= (expected + diff));
}

bool nearExtPattern(float r, float g, float b, float lum, float expR, float expG, float expB, float expLum) {
  return nearColorComponent(r, expR) && nearColorComponent(g, expG) && nearColorComponent(b, expB) &&
     nearLumosityComponent(lum, expLum);
}

bool nearPattern(float r, float g, float b, float expR, float expG, float expB) {
  return nearColorComponent(r, expR) && nearColorComponent(g, expG) && nearColorComponent(b, expB);
}

void readColor(AwaStaticClient* awaClient) {

  uint16_t clear, red, green, blue;
  if (color_click_get_color(&clear, &red, &green, &blue) != 0) {
    return;
  }
  uint16_t max = red;
  max = max < green ? green : max;
  max = max < blue ? blue : max;

  float rcoff = 100 * ((float)red) / max;
  float gcoff = 100 * ((float)green) / max;
  float bcoff = 100 * ((float)blue) / max;
  float lum = 0.2126f * (((float)red) / 65535.0f) +
              0.7152f * (((float)green) / 65535.0f) +
              0.0722f * (((float)blue) / 65535.0f);

  int8_t prevCol = lastColour;
  char tmpColor[15];
  memset(tmpColor, 0, sizeof(tmpColor));

  //detect colours
  if (nearPattern(rcoff, gcoff, bcoff, 100, 56, 27)) {
    //strcpy(currentColour, "yellow");
    strcpy(tmpColor, "yellow");
    lastColour = 1;

  } else if (nearPattern(rcoff, gcoff, bcoff, 44, 100, 49)) {
    //strcpy(currentColour, "green");
    strcpy(tmpColor, "green");
    lastColour = 2;

  } else if (nearPattern(rcoff, gcoff, bcoff, 66, 88, 100)) {
    //strcpy(currentColour, "blue");
    strcpy(tmpColor, "blue");
    lastColour = 3;

  } else if (nearPattern(rcoff, gcoff, bcoff, 100, 15, 14)) {
    //strcpy(currentColour, "orange");
    strcpy(tmpColor, "orange");
    lastColour = 4;

  } else if (nearExtPattern(rcoff, gcoff, bcoff, lum, 100, 30, 30, 0.109f)) {
    //strcpy(currentColour, "purple");
    strcpy(tmpColor, "purple");
    lastColour = 5;

  } else {
    //strcpy(currentColour, "none");
    strcpy(tmpColor, "none");
    lastColour = 0;
  }

  if (prevCol != lastColour) {
    //add debug things if needed
    // sprintf(currentColour, "[%s] %d,%d,%d - %d, %d, %d, %d", tmpColor, red, green, blue, (int)(100*rcoff),
    //    (int)gcoff, (int)bcoff, (int)(1000*lum));

    sprintf(currentColour, "%s", tmpColor);
    //send awa notification about change
    AwaStaticClient_ResourceChanged(awaClient, IPSO_COLOUR_OBJECT, 0, IPSO_COLOUR);

    printf("[%s] %d,%d,%d - %d, %d, %d, %d\n",tmpColor, red, green, blue, (int)rcoff, (int)gcoff,
      (int)bcoff, (int)(1000*lum));
  }

  //dump on console
  // printf("[%s] %d,%d,%d - %d, %d, %d, %d\n",tmpColor, red, green, blue, (int)rcoff, (int)gcoff,
  //   (int)bcoff, (int)(1000*lum));
}

void setupIPSO(AwaStaticClient* awaClient) {
  //define IPSO object
  AwaError error = AwaStaticClient_DefineObject(awaClient, IPSO_COLOUR_OBJECT, "Colour", 0, 1);
  if (error != AwaError_Success)
  {
      printf("Failed to register Colour IPSO object\n");
      return;
  }

  //Define resource
  error = AwaStaticClient_DefineResource(awaClient, IPSO_COLOUR_OBJECT, IPSO_COLOUR, "Colour",
    AwaResourceType_String, 0, 1, AwaResourceOperations_ReadOnly);

  AwaStaticClient_SetResourceStorageWithPointer(awaClient, IPSO_COLOUR_OBJECT, IPSO_COLOUR,
    currentColour, sizeof(currentColour), 0);

  if (error != AwaError_Success)
  {
    printf("Failed to define 'Colour' resource\n");
    return;
  }
  AwaStaticClient_CreateObjectInstance(awaClient, IPSO_COLOUR_OBJECT, 0);
  AwaStaticClient_CreateResource(awaClient, IPSO_COLOUR_OBJECT, 0, IPSO_COLOUR);
}

PROCESS_THREAD(ewdemo_process, ev, data)
{
   PROCESS_BEGIN();

   uip_ipaddr_t iaddr;
   uip_ip6addr(&iaddr, GLOBAL_IPv6_ADDR1, GLOBAL_IPv6_ADDR2, GLOBAL_IPv6_ADDR3,
      GLOBAL_IPv6_ADDR4, GLOBAL_IPv6_ADDR5, GLOBAL_IPv6_ADDR6, GLOBAL_IPv6_ADDR7,
      1);
   uip_ds6_defrt_add(&iaddr, 0);

   uip_nameserver_update(&iaddr, UIP_NAMESERVER_INFINITE_LIFETIME);

   // Set colour LED to white
   gpio_init(GPIO_AN);
   gpio_init(GPIO_CS);
   gpio_init(GPIO_PWM);

   gpio_set_direction(GPIO_AN, GPIO_OUTPUT);
   gpio_set_direction(GPIO_CS, GPIO_OUTPUT);
   gpio_set_direction(GPIO_PWM, GPIO_OUTPUT);

   gpio_set_value(GPIO_AN, 1);
   gpio_set_value(GPIO_CS, 1);
   gpio_set_value(GPIO_PWM, 1);

   strcpy(currentColour, "none");

   static AwaStaticClient * awaClient = NULL;
   awaClient = AwaStaticClient_New();

   srand(time(NULL));
   int port = 6000 + (rand() % 32768);

   AwaStaticClient_SetLogLevel(AwaLogLevel_Debug);
   AwaStaticClient_SetEndPointName(awaClient, "ColorDetector");
   AwaStaticClient_SetPSK(awaClient, IDENTITY, binaryKey, sizeof(binaryKey));
   AwaStaticClient_SetCoAPListenAddressPort(awaClient, "::",  port);
   AwaStaticClient_SetBootstrapServerURI(awaClient, DS_SERVER);
   AwaStaticClient_Init(awaClient);

   GPIO_CONFIGURE_AS_DIGITAL(LED1_PORT, LED1_PIN);
   GPIO_CONFIGURE_AS_OUTPUT(LED1_PORT, LED1_PIN);
   GPIO_CONFIGURE_AS_DIGITAL(LED2_PORT, LED2_PIN);
   GPIO_CONFIGURE_AS_OUTPUT(LED2_PORT, LED2_PIN);

   setupIPSO(awaClient);

   static struct etimer et;
   i2c_init();

   static int enableResult = 0;
   enableResult = color_click_enable();
   if (enableResult != 0) {
       GPIO_SET(LED1_PORT, LED1_PIN);
       GPIO_SET(LED2_PORT, LED2_PIN);
       for(;;){}
   }

   while (1)
   {
       waitTime = AwaStaticClient_Process(awaClient);
       etimer_set(&et, (waitTime * CLOCK_SECOND) / 1000);
       PROCESS_YIELD();

       readColor(awaClient);
   }

   AwaStaticClient_Free(&awaClient);
   PROCESS_END();
}
