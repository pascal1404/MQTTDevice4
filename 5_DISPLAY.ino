void initDisplay()
{
  // nextion.command("rest");
  p0indButton.touch(inductionCallback);
  p1indButton.touch(inductionCallback);
  p0kettleButton.touch(kettleCallback);
  p2kettleButton.touch(kettleCallback);
  p1brewButton.touch(brewCallback);
  p2brewButton.touch(brewCallback);
  powerButton.touch(powerButtonCallback);
  activePage = startPage;
  switch (startPage)
  {
  case 0:
//    nextion.command("page 0");
    break;
  case 1:
 //   nextion.command("page 1");
    break;
  case 2:
  //  nextion.command("page 2");
    break;
  default:
  //  nextion.command("page 1");
    break;
  }
}

void dispPublishmqtt()
{
  if (pubsubClient.connected())
  {
    StaticJsonDocument<4> doc;
    char jsonMessage[2];
    serializeJson(doc, jsonMessage);
    DEBUG_MSG("%s\n", "Disp: Request CBPi4 configuration");
    pubsubClient.publish("cbpi/updatekettle", jsonMessage);
    pubsubClient.publish("cbpi/updateactor", jsonMessage);
    pubsubClient.publish("cbpi/updatesensor", jsonMessage);
    pubsubClient.publish("cbpi/updatefermenter", jsonMessage);
  }
}

void BrewPage()
{
  // DEBUG_MSG("Disp: BrewPage activeBrew: %d kettleID0: %s\n", activeBrew, structKettles[0].id);
  currentStepName_text.attribute("txt", currentStepName);
  currentStepRemain_text.attribute("txt", currentStepRemain);
  nextStepRemain_text.attribute("txt", nextStepRemain);
  nextStepName_text.attribute("txt", nextStepName);
  int kettles = 0;
  showHeader();

  counter += 1;
  if(counter % 4 == 0)
  {
    dispIdx += 1;
  }  
  
  updateDisplay:
  if (strlen(structKettles[0].id) > 0)
  {
    kettles = 1;
    kettleName1_text.attribute("txt", structKettles[0].name);
    kettleSoll1_text.attribute("txt", structKettles[0].target_temp);
    kettleIst1_text.attribute("txt", structKettles[0].current_temp);
    if (dispIdx == 0)
    {
      homeScreen(currentStepName, currentStepRemain, structKettles[0].target_temp, structKettles[0].current_temp, structKettles[0].name);
    }
  }
  else
    dispPublishmqtt();

  if (strlen(structKettles[1].id) > 0)
  {
    kettles = 2;
    kettleName2_text.attribute("txt", structKettles[1].name);
    kettleSoll2_text.attribute("txt", structKettles[1].target_temp);
    kettleIst2_text.attribute("txt", structKettles[1].current_temp);
    if (dispIdx == 1)
    {
      homeScreen(currentStepName, currentStepRemain, structKettles[1].target_temp, structKettles[1].current_temp, structKettles[1].name);
    }
  }
  if (strlen(structKettles[2].id) > 0)
  {
    kettles = 3;
    kettleName3_text.attribute("txt", structKettles[2].name);
    kettleSoll3_text.attribute("txt", structKettles[2].target_temp);
    kettleIst3_text.attribute("txt", structKettles[2].current_temp);
    if (dispIdx == 2)
    {
      homeScreen(currentStepName, currentStepRemain, structKettles[2].target_temp, structKettles[2].current_temp, structKettles[2].name);
    }
  }
  if (strlen(structKettles[3].id) > 0)
  {
    kettles = 4;
    kettleName4_text.attribute("txt", structKettles[3].name);
    kettleSoll4_text.attribute("txt", structKettles[3].target_temp);
    kettleIst4_text.attribute("txt", structKettles[3].current_temp);
    if (dispIdx == 3)
    {
      homeScreen(currentStepName, currentStepRemain, structKettles[3].target_temp, structKettles[3].current_temp, structKettles[3].name);
    }
  }
  
  if (dispIdx >= kettles)
  {
    dispIdx = 0;
    goto updateDisplay;
  }
  
  slider.value(sliderval);
  notification.attribute("txt", notify);

if (inductionCooker.isEnabled)
{
  showDispInd();
}
else{
  actorstatus();
      }
showFooter();

}

void KettlePage()
{
  // DEBUG_MSG("Disp: KettlePage activeBrew: %d kettleID0: %s\n", activeBrew, structKettles[0].id);

showHeader();

  if (strlen(structKettles[0].sensor) != 0)
  {
    for (int i = 0; i < numberOfSensors; i++)
    {
      if (strcmp(structKettles[i].sensor, sensors[0].getId().c_str()) == 0)
      {
        p1temp_text.attribute("txt", structKettles[i].current_temp);
        p1target_text.attribute("txt", structKettles[i].target_temp);
        // DEBUG_MSG("Display: KettlePage Sensor ID: %s\n", structKettles[i].sensor);
        
        homeScreen(currentStepName, currentStepRemain, structKettles[i].target_temp, structKettles[i].current_temp, structKettles[i].name);
        break;
      }
    }
  }
  else
  {
    p1temp_text.attribute("txt", structKettles[0].current_temp);
    p1target_text.attribute("txt", "0");
    
    homeScreen(currentStepName, currentStepRemain, "--", structKettles[0].current_temp, structKettles[0].name);
  }

if (inductionCooker.isEnabled)
{
  showDispInd();
}
else{
  actorstatus();
      }
showFooter();
  
  p1current_text.attribute("txt", currentStepName);
  p1remain_text.attribute("txt", currentStepRemain);
  p1slider.value(sliderval);
  p1notification.attribute("txt", notify);
}

void InductionPage()
{
  // DEBUG_MSG("Disp: InductionPage activeBrew: %d kettleID0: %s\n", activeBrew, structKettles[0].id);
  // p2uhrzeit_text
  // p2slider
  // p2temp_text
  // 316 = 0°C - 360 = 44°C - 223 = 100°C -- 53,4 je 20°C

  int32_t aktSlider = p2slider.value();

  if (aktSlider >= 0 && aktSlider <= 100)
    inductionCooker.handleInductionPage(aktSlider);

  if ((sensors[0].getValue() + sensors[0].getOffset()) < 16.0)
  {
    p2gauge.attribute("val", (int)((sensors[0].getValue() + sensors[0].getOffset()) * 2.7 + 316));
  }
  else
  {
    p2gauge.attribute("val", (int)((sensors[0].getValue() + sensors[0].getOffset()) * 2.7 - 44));
  }
  p2temp_text.attribute("txt", strcat(structKettles[0].current_temp, "°C"));
}

void cbpi4kettle_subscribe()
{
  if (pubsubClient.connected())
  {
    // DEBUG_MSG("Disp: Subscribing to %s\n", cbpi4kettle_topic);
    pubsubClient.subscribe(cbpi4kettle_topic);
    pubsubClient.loop();
  }
}

void cbpi4kettle_unsubscribe()
{
  if (pubsubClient.connected())
  {
    DEBUG_MSG("Disp: Unsubscribing from %s\n", cbpi4kettle_topic);
    pubsubClient.unsubscribe(cbpi4steps_topic);
  }
}

void cbpi4steps_subscribe()
{
  if (pubsubClient.connected())
  {
    DEBUG_MSG("Disp: Subscribing to %s\n", cbpi4steps_topic);
    pubsubClient.subscribe(cbpi4steps_topic);
    pubsubClient.loop();
  }
}

void cbpi4steps_unsubscribe()
{
  if (pubsubClient.connected())
  {
    DEBUG_MSG("Disp: Unsubscribing from %s\n", cbpi4steps_topic);
    pubsubClient.unsubscribe(cbpi4steps_topic);
  }
}

void cbpi4notification_subscribe()
{
  if (pubsubClient.connected())
  {
    DEBUG_MSG("Disp: Subscribing to %s\n", cbpi4notification_topic);
    pubsubClient.subscribe(cbpi4notification_topic);
    pubsubClient.loop();
  }
}
void cbpi4notification_unsubscribe()
{
  if (pubsubClient.connected())
  {
    DEBUG_MSG("Disp: Unsubscribing from %s\n", cbpi4notification_topic);
    pubsubClient.unsubscribe(cbpi4notification_topic);
  }
}

void cbpi4kettle_handlemqtt(char *payload)
{
  StaticJsonDocument<768> doc;
  DeserializationError error = deserializeJson(doc, (const char *)payload);
  if (error)
  {
    int memoryUsed = doc.memoryUsage();
    DEBUG_MSG("Disp: handlemqtt notification deserialize Json error %s MemoryUsage %d\n", error.c_str(), memoryUsed);
    return;
  }
  
  for (int i = 0; i < maxKettles; i++)
  {
    if (strlen(structKettles[i].id) == 0) //structKettle unbelegt
    {
      // DEBUG_MSG("New Kettle setup start %d ID: %s Name: %s Current: %s Target: %s Sensor: %s activepage %d\n", i, structKettles[i].id, structKettles[i].name, structKettles[i].current_temp, structKettles[i].target_temp, structKettles[i].sensor, activePage);
      strlcpy(structKettles[i].id, doc["id"], maxIdSign);
      strlcpy(structKettles[i].name, doc["name"], maxKettleSign);
      dtostrf(doc["target_temp"], 1, 1, structKettles[i].target_temp);
      strlcpy(structKettles[i].sensor, doc["sensor"], maxSensorSign);
      char sensorupdate[45];
      sprintf(sensorupdate, "%s%s", cbpi4sensor_topic, structKettles[i].sensor);
      pubsubClient.subscribe(sensorupdate);
      switch (i)
      {
      case 0:
        kettleName1_text.attribute("txt", structKettles[i].name);
        kettleSoll1_text.attribute("txt", structKettles[i].target_temp);
        break;
      case 1:
        kettleName2_text.attribute("txt", structKettles[i].name);
        kettleSoll2_text.attribute("txt", structKettles[i].target_temp);
        break;
      case 2:
        kettleName3_text.attribute("txt", structKettles[i].name);
        kettleSoll3_text.attribute("txt", structKettles[i].target_temp);
        break;
      case 3:
        kettleName4_text.attribute("txt", structKettles[i].name);
        kettleSoll4_text.attribute("txt", structKettles[i].target_temp);
        break;
      }
      // DEBUG_MSG("New Kettle Setup end %d ID: %s Name: %s Current: %s Target: %s Sensor: %s\n", i, structKettles[i].id, structKettles[i].name, structKettles[i].current_temp, structKettles[i].target_temp, structKettles[i].sensor);
      break;
    }
    else // structKettle belegt
    {
      // DEBUG_MSG("++ OLD kettle ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
      if (structKettles[i].id == doc["id"])
      {
        dtostrf(doc["target_temp"], 1, 1, structKettles[i].target_temp);
        switch (i)
        {
        case 0:
          kettleSoll1_text.attribute("txt", structKettles[i].target_temp);
          break;
        case 1:
          kettleSoll2_text.attribute("txt", structKettles[i].target_temp);
          break;
        case 2:
          kettleSoll3_text.attribute("txt", structKettles[i].target_temp);
          break;
        case 3:
          kettleSoll4_text.attribute("txt", structKettles[i].target_temp);
          break;
        }
        break;
      }
    }
    // DEBUG_MSG("OLD kettle ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
  }
  // DEBUG_MSG("Disp: kettle_handlemqtt %s %s\n", sensorID.c_str(), target_temp.c_str());
}

void cbpi4sensor_handlemqtt(char *payload)
{
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, (const char *)payload);
  if (error)
  {
    int memoryUsed = doc.memoryUsage();
    DEBUG_MSG("Disp: handlemqtt notification deserialize Json error %s MemoryUsage %d\n", error.c_str(), memoryUsed);

    return;
  }
  for (int i = 0; i < maxKettles; i++)
  {
    if (structKettles[i].sensor == doc["id"])
    {
      // DEBUG_MSG("Sensor value PRE %d Sensor-ID: %s Kettle-Sensor: %s value: %s activepage: %d\n", i, structKettles[i].id, structKettles[i].sensor, structKettles[i].current_temp, activePage);
      if (activePage == 0)
      {
        dtostrf(doc["value"], 1, 1, structKettles[i].current_temp);
        switch (i)
        {
        case 0:
          kettleIst1_text.attribute("txt", structKettles[i].current_temp);
          break;
        case 1:
          kettleIst2_text.attribute("txt", structKettles[i].current_temp);
          break;
        case 2:
          kettleIst3_text.attribute("txt", structKettles[i].current_temp);
          break;
        case 3:
          kettleIst4_text.attribute("txt", structKettles[i].current_temp);
          break;
        }
      }
      if (activePage == 1)
        p1temp_text.attribute("txt", structKettles[0].current_temp);
      // DEBUG_MSG("Sensor value POST %d Sensor-ID: %s Kettle-Sensor: %s value: %s activepage %d\n", i, structKettles[i].id, structKettles[i].sensor, structKettles[i].current_temp, activePage);
      break;
    }
  }
  return;
}

void cbpi4steps_handlemqtt(char *payload)
{
  StaticJsonDocument<768> doc;
  DeserializationError error = deserializeJson(doc, (const char *)payload);
  if (error)
  {
    int memoryUsed = doc.memoryUsage();
    DEBUG_MSG("Disp: handlemqtt notification deserialize Json error %s MemoryUsage %d\n", error.c_str(), memoryUsed);

    return;
  }
  if (doc["status"] == "D") // ignore solved steps
    return;

  JsonObject props = doc["props"];
  bool newStep = true;

  for (int i = 0; i < stepsCounter; i++)
  {
    if (structSteps[i].id == doc["id"])
    {
      int minutes = props["Timer"].as<int>() | 0;
      sprintf(structSteps[i].timer, "%02d:%02d", minutes, 0);
      newStep = false;
      break;
    }
  }

  if (newStep == true)
  {
    strlcpy(structSteps[stepsCounter].id, doc["id"], maxIdSign);
    strlcpy(structSteps[stepsCounter].name, doc["name"], maxStepSign);
    int minutes = props["Timer"].as<int>() | 0;
    sprintf(structSteps[stepsCounter].timer, "%02d:%02d", minutes, 0);
    stepsCounter++;
  }

  if (doc["status"] == "A")
  {
    if (!activeBrew) // aktiver Step vorhanden?
      activeBrew = true;

    current_step = true;
    int valTimer = 0;
    int min = 0;
    int sec = 0;
        
    if (currentStepName != doc["name"]) // New active step
    {
      strlcpy(currentStepName, doc["name"] | "", maxStepSign);
      current_step = true;
      for (int i = 0; i < stepsCounter; i++)
      {
        if (structSteps[i].name == doc["name"])
        {
          if (stepsCounter >= i + 1)
          {
            strlcpy(nextStepName, structSteps[i + 1].name, maxStepSign);
            strlcpy(nextStepRemain, structSteps[i + 1].timer, maxRemainSign);
          }
          if (activePage == 0)
          {
            nextStepName_text.attribute("txt", nextStepName);
            nextStepRemain_text.attribute("txt", nextStepRemain);
          }
          break;
        }
      }

      if (doc["type"] == "MashStep")
      {
        strlcpy(notify, "Waiting for Target Temp", maxNotifySign);
      }
      else if (doc["type"] == "ToggleStep")
      {
        strlcpy(notify, "", maxNotifySign);
      }
      else if (doc["type"] == "NotificationStep")
      {
        strlcpy(notify, props["Notification"] | "", maxNotifySign);
      }
      else if (doc["type"] == "WaitStep")
      {
        strlcpy(notify, "WaitStep", maxNotifySign);
      }
      else if (doc["type"] == "TargetStep")
      {
        strlcpy(notify, "", maxNotifySign);
      }
      else if (doc["type"] == "BoilStep")
      {
        strlcpy(notify, "BoilStep", maxNotifySign);
      }
      else if (doc["type"] == "MashInStep")
      {
        strlcpy(notify, "Waiting for Target Temp", maxNotifySign);
      }
      else if (doc["type"] == "CoolDownStep")
      {
        strlcpy(notify, "CoolDownStep", maxNotifySign);
      }
      else if (doc["type"] == "ActorStep")
      {
        strlcpy(notify, "ActorStep", maxNotifySign);
      }
      else if (doc["type"] == "ClearLogStep")
      {
        strlcpy(notify, "", maxNotifySign);
      }
      else
      {
        strlcpy(notify, "", maxNotifySign);
      }

      // DEBUG_MSG("DISP stephandle 1 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
      if (activePage == 0)
        currentStepName_text.attribute("txt", currentStepName);
      if (activePage == 1)
        p1current_text.attribute("txt", currentStepName);
    }
    // DEBUG_MSG("DISP stephandle 2 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
    if (doc["state_text"] != 0 && doc["state_text"] != "Waiting for Target Temp")
    {
      strlcpy(currentStepRemain, doc["state_text"] | "", maxRemainSign);
      if (activePage == 0)
        currentStepRemain_text.attribute("txt", currentStepRemain);
      if (activePage == 1)
        p1remain_text.attribute("txt", currentStepRemain);
      // DEBUG_MSG("DISP stephandle 3 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));

      if (doc["type"] == "MashStep" && doc["state_text"] != "")
      {
        char delimiter[] = ":";
        char buf[10];
        strcpy(buf, doc["state_text"].as<const char *>());
        char *hours = strtok(buf, delimiter);
        char *minutes = strtok(NULL, delimiter);
        char *seconds = strtok(NULL, delimiter);
        valTimer = props["Timer"].as<int>() | 0;
        min = atoi(minutes);
        sec = atoi(seconds);
      }
      // DEBUG_MSG("DISP stephandle 4 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
      if (valTimer > 0 && min > 0)
      {
        sliderval = (valTimer * 60 - (min * 60 + sec)) * 100 / (valTimer * 60);
        if (activePage == 0)
          slider.value(sliderval);
        if (activePage == 1)
          p1slider.value(sliderval);
      }
      else
      {
        sliderval = 0;
        if (activePage == 0)
          slider.value(0);
        if (activePage == 1)
          p1slider.value(0);
      }
      // DEBUG_MSG("DISP stephandle 5 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
      /*
      min = min * 60;
      int akt = min + sec;
      valTimer = valTimer * 60;
      int rest = valTimer - akt;
      rest = rest * 100 / valTimer;
      slider.value(rest);
      */
    }
    else
    {
      // DEBUG_MSG("DISP stephandle 6 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
      if (props.containsKey("Timer"))
      {
        int minutes = props["Timer"].as<int>();
        sprintf(currentStepRemain, "%02d:%02d", minutes, 0);
        if (activePage == 0)
          currentStepRemain_text.attribute("txt", currentStepRemain);
        if (activePage == 1)
          p1remain_text.attribute("txt", currentStepRemain);
        // DEBUG_MSG("DISP stephandle 7 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
      }
      else
      {
        strcpy(currentStepRemain, "0:00");
        if (activePage == 0)
          currentStepRemain_text.attribute("txt", currentStepRemain);
        if (activePage == 1)
          p1remain_text.attribute("txt", currentStepRemain);
        // DEBUG_MSG("DISP stephandle 8 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
      }
      sliderval = 0;
      if (activePage == 0)
        slider.value(sliderval);
      if (activePage == 1)
        p1slider.value(sliderval);
      // DEBUG_MSG("DISP stephandle 9 ActivePage: %d ID: %s Name: %s Sensor: %s strlen: %d\n", activePage, structKettles[0].id, structKettles[0].name, structKettles[0].sensor, strlen(structKettles[0].id));
    }
    return;
  }
}

void cbpi4notification_handlemqtt(char *payload)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, (const char *)payload);
  if (error)
  {
    int memoryUsed = doc.memoryUsage();
    DEBUG_MSG("Disp: handlemqtt notification deserialize Json error %s MemoryUsage %d\n", error.c_str(), memoryUsed);
    return;
  }
  if (doc["title"] == "Stop")
  {
    activeBrew = false;
    if (activePage == 0)
    {
      strlcpy(currentStepName, "BrewPage", maxStepSign);
      strlcpy(currentStepRemain, "", maxRemainSign);
      strlcpy(nextStepName, "", maxStepSign);
      strlcpy(nextStepRemain, "", maxRemainSign);
    }
    if (activePage == 1)
    {
      strlcpy(currentStepName, sensors[0].getName().c_str(), maxStepSign);
    }
    strlcpy(notify, "", maxNotifySign);
    return;
  }
  if (doc["title"] == "Brewing completed")
  {
    activeBrew = false;
    strlcpy(notify, "Brewing completed", maxNotifySign);
    return;
  }
  if (doc["title"] == "Start" || doc["title"] == "Resume")
    activeBrew = true;

  strlcpy(notify, doc["message"] | "", maxNotifySign);
}
// ###################################################################################################################################
// ###################################################################################################################################
// Other Display

void printtext(uint8_t x, uint8_t y, uint16_t color, uint16_t bgcolor, int textsize, String textformat){
  tft.setTextColor(color, bgcolor);
  tft.setTextSize(textsize);
  tft.setCursor(x, y);
  tft.print(textformat);
}

void printtext(uint8_t x, uint8_t y, uint16_t color, uint16_t bgcolor, int textsize, const char* textformat, int var){
  char str[100];
  tft.setTextColor(color, bgcolor);
  tft.setTextSize(textsize);
  tft.setCursor(x, y);
  sprintf(str, textformat, var);
  tft.print(str);
}

void dispStartScreen() // Show Startscreen
{
  showDispClear();
  showDispCbpi();
  showDispIP(String(WiFi.softAPIP().toString()));
  printtext(5, 5, ST77XX_WHITE, ST77XX_BLACK, 1, "AP-Mode: ");
}

void showDispClear() // Clear Display
{
    tft.fillScreen(ST77XX_BLACK);
}

void showDispCbpi() // SHow CBPI icon
{
  showDispClear();  
  tft.setTextWrap(false);
  tft.drawBitmap(10, 30, cbpi_gross_logo, cbpi_gross_logo_width, cbpi_gross_logo_height, ST77XX_GREEN);
}

void showDispWlan() // Show WLAN icon
{
  if (WiFi.status() == WL_CONNECTED)
  {
    tft.drawBitmap(115, 150, wlan_logo, 10, 10, ST77XX_YELLOW);
  }
  else
  {
    tft.drawBitmap(115, 150, wlan_logo, 10, 10, ST77XX_DARKGREY);
  }
}

void showDispMqtt() // SHow MQTT icon
{
  if (mqtt_state)
    tft.drawBitmap(100, 150, mqtt_logo, 10, 10, ST77XX_YELLOW);
  else
  {
    tft.drawBitmap(100, 150, mqtt_logo, 10, 10, ST77XX_DARKGREY);
    showDispErr("MQTT ERROR");
    unsigned long val = 2*wait_on_error_mqtt - (millis() - mqttconnectlasttry);
    if (val > wait_on_error_mqtt )
      return;
    showDispErr2(String(val/1000));
  }
}

void showDispAct() // Show actor status in the Header
{    
    for(int i=0;i<numberOfActors;i++){
      if(actors[i].name_actor.substring(0, 9)=="Ruehrwerk"){
        if (!actors[i].isOn) 
          tft.drawBitmap(i*15, 0, ruehrwerk_logo, 10, 10, ST77XX_DARKGREY);
        else
          tft.drawBitmap(i*15, 0, ruehrwerk_logo, 10, 10, ST77XX_YELLOW);
      }
      else if(actors[i].name_actor.substring(0, 7)=="Luefter"){
        if (!actors[i].isOn)
          tft.drawBitmap(i*15, 0, luefter_logo, 10, 10, ST77XX_DARKGREY);
        else
          tft.drawBitmap(i*15, 0, luefter_logo, 10, 10, ST77XX_YELLOW);
      }
      else if(actors[i].name_actor.substring(0, 5)=="Pumpe"){
        if (!actors[i].isOn)
          tft.drawBitmap(i*15, 0, pumpe_logo, 10, 10, ST77XX_DARKGREY);
        else
          tft.drawBitmap(i*15, 0, pumpe_logo, 10, 10, ST77XX_YELLOW);
      }
      else if(actors[i].name_actor.substring(0, 6)=="Ventil"){
        if (!actors[i].isOn)
          tft.drawBitmap(i*15, 0, ventil_logo, 10, 10, ST77XX_DARKGREY);
        else
          tft.drawBitmap(i*15, 0, ventil_logo, 10, 10, ST77XX_YELLOW);
      }
      else if(actors[i].name_actor.substring(0, 6)=="Heizer"){
        if (!actors[i].isOn)
          tft.drawBitmap(i*15, 0, fire_logo, 10, 10, ST77XX_DARKGREY);
        else
          tft.drawBitmap(i*15, 0, fire_logo, 10, 10, ST77XX_YELLOW);
      }
      else if(actors[i].name_actor.substring(0, 7)=="Kuehler"){
        if (!actors[i].isOn)
          tft.drawBitmap(i*15, 0, kuehler_logo, 10, 10, ST77XX_DARKGREY);
        else
          tft.drawBitmap(i*15, 0, kuehler_logo, 10, 10, ST77XX_YELLOW);
      }      
      else {
        if (!actors[i].isOn)
          tft.drawBitmap(i*15, 0, random_logo, 10, 10, ST77XX_DARKGREY);
        else
          tft.drawBitmap(i*15, 0, random_logo, 10, 10, ST77XX_YELLOW); 
      }
    }
  }

void homeScreen(String Stepname, String timeleft, String STemp, String ITemp, String kettleName){
  // allocate the buffer
  char newStepname[MaxStringGr2]     = { 0 };
  char newtimeleft[MaxStringGr2]     = { 0 };
  char newSTemp[MaxStringGr3]        = { 0 };
  char newITemp[MaxStringGr3]        = { 0 };
  char newKettleName[MaxStringGr2]   = { 0 };
    
  (Stepname + "       ").toCharArray(newStepname, MaxStringGr2);
  (timeleft + "       ").toCharArray(newtimeleft, MaxStringGr2);
  ("T:" + (STemp.length() > 0 ? STemp : "XX.X") + "C ").toCharArray(newSTemp, MaxStringGr3);
  ("C:" + (ITemp.length() > 0 ? ITemp : "XX.X") + "C ").toCharArray(newITemp, MaxStringGr3);
  (kettleName + ":       ").toCharArray(newKettleName, MaxStringGr2);
  
  Serial.print("timeleft: ");
  Serial.println(timeleft);
  
  if (mqtt_state)
  {
    // has the string changed since the last tft update?
    if (strcmp(newStepname,oldStepname) != 0) {
      printtext(0, 11, ST77XX_CYAN, ST77XX_BLACK, 2, newStepname);
      strcpy(oldStepname,newStepname);
    }
  }
  // has the string changed since the last tft update?
  if (strcmp(newtimeleft,oldtimeleft) != 0) {
    printtext(0, 32, ST77XX_RED, ST77XX_BLACK, 2, newtimeleft);
    strcpy(oldtimeleft,newtimeleft);
  }
  // has the string changed since the last tft update?
  if (strcmp(newKettleName,oldKettleName) != 0) {
    printtext(0, 50, ST77XX_YELLOW, ST77XX_BLACK, 2, newKettleName);
    strcpy(oldKettleName,newKettleName);
  }
  // has the string changed since the last tft update?
  if (strcmp(newSTemp,oldSTemp) != 0) {
    printtext(0, 70, ST77XX_GREEN, ST77XX_BLACK, 3, newSTemp);
    strcpy(oldSTemp,newSTemp);
  }
  // has the string changed since the last tft update?
  if (strcmp(newITemp,oldITemp) != 0) {
    printtext(0, 97, ST77XX_GREEN, ST77XX_BLACK, 3, newITemp);
    strcpy(oldITemp,newStepname);
  }
}

void showDispSen(String str) // Show Sensor status on the left
{
 /* char str[15];
  if (numberOfSensors == 0)
  {
    printtext(0, 99, ST77XX_GREEN, ST77XX_BLACK, 3, "No Sens    ");
    return;
  }
  if (sensors[0].sens_err == 0){
    if(lcdDisplay.oldSenVal != (sensors[0].sens_offset + sensors[0].sens_value)){
      lcdDisplay.oldSenVal = (sensors[0].sens_offset + sensors[0].sens_value);
      sprintf(str,"%.1fC  ", lcdDisplay.oldSenVal);*/
      printtext(0, 99, ST77XX_GREEN, ST77XX_BLACK, 3, str + "C      ");
/*    }
  }
  else
    printtext(0, 99, ST77XX_GREEN, ST77XX_BLACK, 3, "Error     ");
*/
}

void showDispInd() // Show InductionCooker status on the right
{
  char newIndStr[MaxStringGr3]    = { 0 };

  // construct the string representation
  sprintf(newIndStr, "Ind %i%%  ", inductionCooker.newPower);
    
  if (inductionStatus == 1)
  {
    if (inductionCooker.isRelayon)
    {
      if (strcmp(newIndStr,oldIndStr) != 0)
      {
        printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "Ind %i%%  ", inductionCooker.newPower);
        sprintf(oldIndStr, "Ind %i%%  ", inductionCooker.newPower);
      }
    }
    else
    {
      if (strcmp("Ind off    ",oldIndStr) != 0)
      {
        printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "Ind off    ");
        sprintf(oldIndStr, "Ind off    ");
      }
    }
  }
  else if (inductionStatus == 0)
  {
    if (strcmp("Ind off    ",oldIndStr) != 0)
    {
      printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "Ind off    ");
      sprintf(oldIndStr, "Ind off    ");
    }
  }
  else
  {
    if (strcmp("Ind Error   ",oldIndStr) != 0)
    {
      printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "Ind Error   ");
      sprintf(oldIndStr, "Ind Error   ");
    }
  }
}

void showDispTime(const String &value) // Show time value in the upper left with fontsize 2
{
  printtext(90,2,ST77XX_WHITE, ST77XX_BLACK,1," "+value.substring(0,(value.length()-3))); // substring w/o seconds
}

void showDispIP(const String &value) // Show IP address under time value with fontsize 1
{
  printtext(0, 150, ST77XX_WHITE, ST77XX_BLACK, 1, value);
}

void showDispErr(const String &value) 
{
  printtext(0, 11, ST77XX_RED, ST77XX_BLACK, 2,value);
}

void showDispErr2(const String &value) 
{
  printtext(20, 27, ST77XX_RED, ST77XX_BLACK, 2, " -" + value + "sec");
}

void actorstatus(){
  bool heating=false;
  bool cooling=false;
  for(int i=0;i<numberOfActors;i++){
    if(actors[i].name_actor.substring(0, 6)=="Heizer"){
        if (!actors[i].isOn)
          heating=false;
        else
          heating=true;
      }
      else if(actors[i].name_actor.substring(0, 7)=="Kuehler"){
        if (!actors[i].isOn)
          cooling=false;
        else
          cooling=true;
      }      
  }
  if(heating&&!cooling){
    if (strcmp("Heating ",oldIndStr) != 0)
    {
      printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "Heating ");
      sprintf(oldIndStr, "Heating ");
    }
  }
  else if(!heating&&cooling){
    if (strcmp("Cooling ",oldIndStr) != 0)
    {
      printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "Cooling ");
      sprintf(oldIndStr, "Cooling ");
    }
  }
  else if(!heating&&!cooling){
    if (strcmp("in Hyst. ",oldIndStr) != 0)
    {
      printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "in Hyst. ");
      sprintf(oldIndStr, "in Hyst. ");
    }
  }
  else {
    if (strcmp("ERROR 9 ",oldIndStr) != 0)
    {
      printtext(0, 123, ST77XX_RED, ST77XX_BLACK, 3, "ERROR 9 ");
      sprintf(oldIndStr, "ERROR 9 ");
    }
  }
     
}

void showHeader()
{
  showDispTime(timeClient.getFormattedTime());
  showDispAct();
}

void showFooter()
{
  char newIPStr[MaxStringGr1]    = { 0 };
  String temp = WiFi.localIP().toString();
  temp.toCharArray(newIPStr, MaxStringGr1);
    
  if (strcmp(newIPStr,oldIPStr) != 0)
  {
    showDispIP(newIPStr);
    strcpy(oldIPStr,newIPStr);
  }
//  manualMode();
  showDispWlan();
  showDispMqtt();
}

