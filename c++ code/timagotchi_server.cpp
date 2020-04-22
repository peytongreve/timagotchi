#include <WiFi.h> // connects to wifi
#include <time.h> // checks the time

char network[] = "MIT GUEST";
char password[] = "";

const int RESPONSE_TIMEOUT = 6000; //ms to wait for response from host
const uint16_t IN_BUFFER_SIZE = 1000; //size of buffer to hold HTTP request
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP response
char request_buffer[IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response_buffer[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response

// this code is just supposed to post from the timagotchi class to our endpoints

void add_timagotchi(Timagotchi tim) {
    char body[200];
    sprintf(body, "user=%s&gpa=%f&happiness=%d&hunger=%d&major=%d&unitsIn=%d&
                        totalUnits=%d&urop=%d&extracurriculars=%s&numExtracurriculars=%d",
                            tim.user, tim.gpa, tim.happiness, time.hunger, tim.major, tim.class_units,
                                tim.total_units, tim.urop, tim.extracurriculars, tim.num_ecs);
    int body_len = strlen(body);
    sprintf(request_buffer, "POST http://608dev-2.net/sandbox/sc/pgreve/ HTTP/1.1\r\n");  // TODO GET ENDPOINT
    strcat(request_buffer, "Host: 608dev-2.net\r\n");
    strcat(request_buffer, "Content-Type: application/x-www-form-urlencoded\r\n");
    sprintf(request_buffer + strlen(request_buffer), "Content-Length: %d\r\n", body_len);
    strcat(request_buffer, "\r\n");
    strcat(request_buffer, body);
    strcat(request_buffer, "\r\n");
    do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
}

void update_timagotchi(Timagotchi tim) {
    char body[200];
    sprintf(body, "user=%s&gpa=%f&happiness=%d&hunger=%d&major=%d&unitsIn=%d&
                        totalUnits=%d&urop=%d&extracurriculars=%s&numExtracurriculars=%d",
                            tim.user, tim.gpa, tim.happiness, time.hunger, tim.major, tim.class_units,
                                tim.total_units, tim.urop, tim.extracurriculars, tim.num_ecs);
    int body_len = strlen(body);
    sprintf(request_buffer, "POST http://608dev-2.net/sandbox/sc/pgreve/ HTTP/1.1\r\n");  // TODO GET ENDPOINT
    strcat(request_buffer, "Host: 608dev-2.net\r\n");
    strcat(request_buffer, "Content-Type: application/x-www-form-urlencoded\r\n");
    sprintf(request_buffer + strlen(request_buffer), "Content-Length: %d\r\n", body_len);
    strcat(request_buffer, "\r\n");
    strcat(request_buffer, body);
    strcat(request_buffer, "\r\n");
    do_http_request("608dev-2.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
}

bool check_decrement_hunger(Timagotchi tim) {
    time_t now = time(NULL);
    double seconds = difftime(now, tim.last_time_fed);
    if (seconds > 288000) {
        return true;
    }
    return false;
}

/*----------------------------------
 * char_append Function:
 * Arguments:
 *    char* buff: pointer to character array which we will append a
 *    char c: 
 *    uint16_t buff_size: size of buffer buff
 *    
 * Return value: 
 *    boolean: True if character appended, False if not appended (indicating buffer full)
 */
uint8_t char_append(char* buff, char c, uint16_t buff_size) {
        int len = strlen(buff);
        if (len>buff_size) return false;
        buff[len] = c;
        buff[len+1] = '\0';
        return true;
}

/*----------------------------------
 * do_http_request Function:
 * Arguments:
 *    char* host: null-terminated char-array containing host to connect to
 *    char* request: null-terminated char-arry containing properly formatted HTTP request
 *    char* response: char-array used as output for function to contain response
 *    uint16_t response_size: size of response buffer (in bytes)
 *    uint16_t response_timeout: duration we'll wait (in ms) for a response from server
 *    uint8_t serial: used for printing debug information to terminal (true prints, false doesn't)
 * Return value:
 *    void (none)
 */
void do_http_request(char* host, char* request, char* response, uint16_t response_size, uint16_t response_timeout, uint8_t serial){
  WiFiClient client; //instantiate a client object
  if (client.connect(host, 80)) { //try to connect to host on port 80
    if (serial) Serial.print(request);//Can do one-line if statements in C without curly braces
    client.print(request);
    memset(response, 0, response_size); //Null out (0 is the value of the null terminator '\0') entire buffer
    uint32_t count = millis();
    while (client.connected()) { //while we remain connected read out data coming back
      client.readBytesUntil('\n',response,response_size);
      if (serial) Serial.println(response);
      if (strcmp(response,"\r")==0) { //found a blank line!
        break;
      }
      memset(response, 0, response_size);
      if (millis()-count>response_timeout) break;
    }
    memset(response, 0, response_size);  
    count = millis();
    while (client.available()) { //read out remaining text (body of response)
      char_append(response,client.read(),OUT_BUFFER_SIZE);
    }
    if (serial) Serial.println(response);
    client.stop();
    if (serial) Serial.println("-----------");  
  }else{
    if (serial) Serial.println("connection failed :/");
    if (serial) Serial.println("wait 0.5 sec...");
    client.stop();
  }
}