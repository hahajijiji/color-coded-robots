// The way I have setup this program to read the yellow value precisely 
// is by using the distance formula for a 3D basis which can be read over in the link below
// https://en.wikipedia.org/wiki/Euclidean_distance
// by assuming that each color filter variable is like X,Y,Z on a traditional 3D plane
// we can use this formula to create a linear and accurate method to create a threshold
// value that will not interfere with other color readings




long yell = 0;//holds the value of the distance between the read values and pre-recorded values for yellow


int PWM_PIN = 2;// Color sensor pin 
int s2 = 23;// s3 and s2 to select the color filter
int s3 = 24; 

int redFilter = 0;//holds red filter value
int blueFilter = 0;//holds blue filter value
int greenFilter = 0;//holds green filter value

int yellow_red = 75;//these values are the RGB values for the filter that I found for yellow
int yellow_blue = 176;//these values will differ based on lighting and distance of the colored object
int yellow_green = 95;



int readColor(void){
  digitalWrite(s2, LOW);//find the red filter value
  digitalWrite(s3, LOW);
  redFilter = pulseIn(PWM_PIN, HIGH);
  
  digitalWrite(s2, LOW);//find the blue filter value
  digitalWrite(s3, HIGH);
  blueFilter = pulseIn(PWM_PIN, HIGH);
   
  digitalWrite(s2, HIGH);//find teh green filter value
  digitalWrite(s3, HIGH);
  greenFilter = pulseIn(PWM_PIN, HIGH);

Serial.print(greenFilter);//this was used to debug, this entire section can be removed if need be
Serial.print("\n");
Serial.print(redFilter);
Serial.print("\n");
Serial.print(blueFilter);
Serial.print("\n");

if (yell < 250){    //the value 250 was determined by addressing the yell value given all other colors
  Serial.println("IT'S YELLOW!!! \n");//each color has a Serial.print for troubleshooting 
  
  }

 else if (greenFilter > redFilter)
  {
    if (blueFilter > redFilter)
    {
      Serial.println("IT'S RED!!!");
    }

     else if (redFilter > blueFilter)
        {
        Serial.println("IT'S BLUE!!! ");
        }
      }
    
    
  else if(greenFilter > blueFilter)
  {
    Serial.println("IT'S BLUE!!!");
  }

   else
      {
      Serial.println("IT'S GREEN!!!");
      }
  //below is the meat of it, R,G,& B are decided by finding the greatest valued filter variable, but this doesn't work with yellow
  // each pre-recorded value is subtracted by the current respective value, squared, added to the others, and then the square root is taken
  // this gives an accurate and linear means to find yellow without overrunning the RGB values, and doesn't require that we do similar reads 
  //with RGB, only non RGB is necessary untill distinction via this method is not possible due to overlap in the 3D space
  Serial.println("\n");//space out vlaues
  yell =  (((yellow_red - redFilter)* (yellow_red - redFilter) + (yellow_blue - blueFilter)*(yellow_blue - blueFilter) + (yellow_green - greenFilter)*(yellow_green - greenFilter)));
  yell = sqrt(yell);
  Serial.println("Yellow data: ");
  Serial.print(yell);
  Serial.println("\n");
  }


 
 
void setup() {
  pinMode(PWM_PIN, INPUT);
  Serial.begin(115200);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
}

void loop() {
 readColor();
 delay(500);
  
}
