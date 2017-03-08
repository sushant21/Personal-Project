struct node
  {
    double x;
    double y;
    int dir[8]={0};
                                  //   struct for an intersection node
  };
int nodes_discovered=0,
    ang=0,
    curr_node;                      
double  x_coord=0,
        y_coord=0,
        x2_coord=0,
        y2_coord=0,
        x3_coord=0,
        y3_coord=0;
bool  sensorval[4],
      lsensorval,
      rsensorval,
      right_turn,
      left_turn,
      straight,
      diag_left,
      diag_right,
      fsensorval,
      state_enc,
      state_enc2,
      new_node=0,
      upast=0,
      tpast=0,
      run_mode=0;
node junctions[22];
/*void intersection();
 bool not_visited();
  void init_node();
void turn_left(bool);
void lu_turn(bool);
void turn_right(bool);
void ru_turn(bool);*/
void setup() 
{ 
  pinMode(9,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(6,OUTPUT);

  
  pinMode(11,INPUT);
  pinMode(10,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(2,INPUT);
  pinMode(12,INPUT);
  pinMode(4,INPUT);
  pinMode(13,INPUT);
  pinMode(A3,INPUT);
  pinMode(A0,INPUT);
  pinMode(A5,OUTPUT);
  pinMode(0,INPUT);
  digitalWrite(A5,LOW);
  state_enc=0;
  state_enc2=0;
  Serial.begin(9600); 
  
 
  
  while( digitalRead(A3)==0 );
  
  
  delay(500);
  
  
 
}
void read_values()  //read input from all IR sensors
{
  sensorval[0]=!digitalRead(11);
  sensorval[1]=!digitalRead(10);
  sensorval[2]=!digitalRead(7);
  sensorval[3]=!digitalRead(8);
  lsensorval=!digitalRead(2);
  rsensorval=!digitalRead(12);
  fsensorval=!digitalRead(4); 
}
 
void loop() 
{
  
   read_values();
   if(digitalRead(13)!=state_enc)
   {
    state_enc=(!state_enc);
    switch(ang)
      {
       case 0:
        y_coord+=1;
      break;
      case 90:
        x_coord+=1;
      break;
      case 180:
        y_coord-=1;
      break;
      case 270:
        x_coord-=1;
      break;
      case 45:
        x_coord+=(1/1.414);
        y_coord+=(1/1.414);
      break;
      case 135:
        x_coord+=(1/1.414);
        y_coord-=(1/1.414);
      break;
      case 225:
        x_coord-=(1/1.414);
        y_coord-=(1/1.414);
      break;
      case 315:
        x_coord-=(1/1.414);
        y_coord+=(1/1.414);
      break;
      }
   }
    if(digitalRead(0)!=state_enc2)
   {
    state_enc2=(!state_enc2);
    switch(ang)
      {
       case 0:
        y2_coord+=1;
      break;
      case 90:
        x2_coord+=1;
      break;
      case 180:
        y2_coord-=1;
      break;
      case 270:
        x2_coord-=1;
      break;
      case 45:
        x2_coord+=(1/1.414);
        y2_coord+=(1/1.414);
      break;
      case 135:
        x2_coord+=(1/1.414);
        y2_coord-=(1/1.414);
      break;
      case 225:
        x2_coord-=(1/1.414);
        y2_coord-=(1/1.414);
      break;
      case 315:
        x2_coord-=(1/1.414);
        y2_coord+=(1/1.414);
      break;
      }
   }
   x3_coord=(x_coord+x2_coord)/2;
   y3_coord=(y_coord+y2_coord)/2;
  if(sensorval[1]==1 && sensorval[2]==1)
  {
    gaadirun(120,120);
  }
  else if((sensorval[1]==1) || (sensorval[0]==1 && sensorval[3]==0))
  {
    gaadirun(20,120);
  }
  else if((sensorval[2]==1) || (sensorval[0]==0 && sensorval[3]==1))
  {
    gaadirun(120,20);
  }
  else if(sensorval[0]==0 && sensorval[3]==0)
  {
    ang=(ang+180)%360;
    upast=1;

    u_turn(); 
    
  }
  if(lsensorval==1 || rsensorval==1) 
  {
    bool ltemp=lsensorval;
    bool rtemp=rsensorval;
    delay(10);
    read_values();
    if(lsensorval==0) lsensorval=ltemp;
    if(rsensorval==0) rsensorval=rtemp;
    gaadirun(120,120);
    if(upast==1){
      x_coord = junctions[curr_node-1].x;
      y_coord = junctions[curr_node-1].y;
      x2_coord = junctions[curr_node-1].x;
      y2_coord = junctions[curr_node-1].y;
    
    }
     intersection();  //we have arrived at an intersection
 
 }
 delay(10);

 
}

void intersection()
{
  bool  dd=0,
        rtemp=0,
        ltemp=0;
        
  
  right_turn=0,
  left_turn=0,
  straight=0,
  diag_left=0,
  diag_right=0;
  
  start:
  
  // now we will determine all the turns available at this intersection
  if(lsensorval==1 && fsensorval==1 && rsensorval==1)
  {    
          left_turn=1;
          right_turn=1;
          while(1)
          {   
              read_values();
              if (lsensorval==0 && rsensorval==0)                                                            
                  break;
              if(lsensorval ==1 && rsensorval==1 && fsensorval==1 && sensorval[0]==1 && sensorval[1]==1 && sensorval[2]==1 && sensorval[3]==1)
              {
                  delay(250);
                  gaadirun(0,0);
                  digitalWrite(A5,HIGH);
                  while(digitalRead(A3)==0);
                  digitalWrite(A5,LOW);
                  run_mode=1;
                  upast=0;
                  state_enc=0;
                  state_enc2=0;
                  x_coord=0;
                  y_coord=0;
                  x2_coord=0;
                  y2_coord=0;
                  x3_coord=0;
                  y3_coord=0;
                  nodes_discovered=0;
                  curr_node=0;
                  ang=0;      
                  return;
              }       
          }
          if(fsensorval==1)
            straight=1;
          delay(70); 
  }
  else if(lsensorval==1 && fsensorval==0 && rsensorval==1)
  {
          diag_left=1;
          diag_right=1;
  }
  else if(lsensorval==1 && fsensorval==1 && rsensorval==0)
  {
          left_turn=1;
          while(1)
          {   
              read_values();
              if(rsensorval==1)
              {
                lsensorval=1;
                fsensorval=1;
                goto start;              
              }
              if (lsensorval==0 || fsensorval==0)
                break;
          }
          while(rtemp==0)
          {
              read_values();
              if(rsensorval==1)
              { 
                  rtemp=1; 
                 
              }
              if(sensorval[0]==1 && sensorval[1]==1)
                  break;
          }
          while(rtemp==0)
          {
              read_values();
              if(rsensorval==1)
              { 
                  rtemp=1;
              }
              if(fsensorval==1)
              {   
                  straight=1;
                  break;
              }
              if(sensorval[0]==0&&sensorval[1]==0)  
                  break;
              if(sensorval[1]==1&&sensorval[2]==0) gaadirun(20,150);
              else if(sensorval[1]==0&&sensorval[2]==1) gaadirun(150,20);
              else gaadirun(150,150);
         }
         gaadirun(150,150);
         delay(10);
         read_values();
         if(rsensorval==1) 
            rtemp=1;
         rsensorval=rtemp;
         if(rsensorval==1)
         {
            diag_right=1;
            straight=0;
         }
         else if(fsensorval==1)
            straight=1;
     }
     else if(lsensorval==0 && fsensorval==1 && rsensorval==1)
     {
          right_turn=1;
          while(1)
          {   
              read_values();
              if(lsensorval==1)
              {
                rsensorval=1;
                fsensorval=1;
                goto start;
              }
              if ((rsensorval==0)||(fsensorval==0))
                break;    
          }
          while(ltemp==0)
          {
              read_values();
              if(lsensorval==1) 
              {
                ltemp=1;    
              }
              if(sensorval[2]==1 && sensorval[3]==1)
                break;
          }
          while(ltemp==0)
          {
              read_values();
              if(lsensorval==1)
              {
                  ltemp=1;
              }
             if (fsensorval==1)
             {
                  straight=1;  
                  break;
             }
             if(sensorval[2]==0&&sensorval[3]==0)
                 break;
                 if(sensorval[2]==1&&sensorval[1]==0) 
                    gaadirun(150,20);
                    else if (sensorval[2]==0&&sensorval[1]==1) gaadirun(20,150);
                 else 
                    gaadirun(150,150);
          }
          gaadirun(150,150);
          delay(10);
          read_values();
          if(lsensorval==1) 
              ltemp=1;
          lsensorval=ltemp;
          if(lsensorval==1)
          {
              diag_left=1;
              straight=0;
          }
          else if(fsensorval==1)
              straight=1;
     }
     else if(lsensorval==1 && fsensorval==0 && rsensorval==0)
     {
          diag_left=1;
          while(lsensorval)
          {
              read_values();
              if (rsensorval) 
              {
                  diag_right=1;
                  break;
              }
          }
     }
     else if(lsensorval==0 && fsensorval==0 && rsensorval==1)
     {
          diag_right=1;
          while(rsensorval==1)
          {
              read_values();
              if (lsensorval==1) 
              {
                  diag_left=1;
                  break;
              }
          }
     }
     delay(map(analogRead(A0),0,1023,150,250));    //change according to battery power
     gaadirun(0,0);
     delay(100);
     read_values();
     if (fsensorval==1&&(sensorval[1]==1||sensorval[2]==1)) 
        straight=1;
        if(sensorval[0]==0&&sensorval[1]==0&&sensorval[2]==0&&sensorval[3]==0&&fsensorval==0) straight=0;  
     else straight=1;
     if(lsensorval==1)
     {
        diag_left=1;
     }
     if(rsensorval==1)
     {
       diag_right=1;
     }
     if(diag_left==1||diag_right==1) straight=0;
     new_node=1;
     bool is_junc=0;
     char val;
     if((straight&&left_turn)||(straight&&right_turn)||(straight&&diag_left)||(straight&&diag_right)||(diag_left&&diag_right)||(diag_left&&right_turn)||(diag_right&&left_turn)||(left_turn&&right_turn))
     {
      is_junc=1;  //the intersection is a junction, meaning there are more than one route possible (due to limited memory, we are storing only these)
     if(run_mode==0)
     {
     if(not_visited()) //if node has not already been discovered, initialise new node 
      {
        nodes_discovered++; 
        init_node();
        curr_node=nodes_discovered;
      } 
      upast=0;
     tpast=0; //these counters are to keep track of uturns and turns where junction was not found(only one way)
     if(junctions[curr_node-1].dir[(ang/45+4)%8]<3) junctions[curr_node-1].dir[(ang/45+4)%8]++;
     val=choose_turn();
     }
     else
    {
      val=get_next_direction();  //find out which next direction to choose, based on directions already taken in the past (stored in the node structure)
      is_junc=0;
    }
     }
     else 
     {
     tpast=1;
    val=choose_turn(); //if visiting for first time, this function is called
    is_junc=0;
    }
    
    
    //now based on value returned by get_next_direction and choose_turn functions, choose appropriate direction
   //also update current direction and node structure accordingly  
  if(val == 'l')
     {
        if(diag_left==1)
        {
            if (is_junc==1) junctions[curr_node-1].dir[(ang/45+7)%8]++;
            ang=(ang+315)%360;
            if(diag_right==0&&right_turn==0) 
                dd=1;
            else 
                dd=0;
        }
        else if(left_turn==1) 
          {
            if (is_junc==1) junctions[curr_node-1].dir[(ang/45+6)%8]++;
            ang=(ang+270)%360;
           }
        if(straight==1) turn_left(dd);
        else lu_turn(dd);
     }
     else if(val=='r')
     {
         if(diag_right==1)
         {
            if (is_junc==1) junctions[curr_node-1].dir[(ang/45+1)%8]++;
            ang=(ang+45)%360;
            if(diag_left==0&&left_turn==0) 
                dd=1;
            else 
                dd=0;
           }
          else if(right_turn==1)
          { 
           if(is_junc==1)  junctions[curr_node-1].dir[(ang/45+2)%8]++;
              ang=(ang+90)%360;
          }
          if(straight==1) 
              turn_right(dd);
          else 
              ru_turn(dd);
     }
     else if(val=='s')
     {
       if(is_junc==1) junctions[curr_node-1].dir[(ang/45)%8]++;
        return;
     }
     else if(val=='p')
     {
      if(is_junc==1)junctions[curr_node-1].dir[(ang/45+4)%8]++;
      ang=(ang+180)%360;
      return;
     }
         
}
void gaadirun(int a,int b) //function to drive the car 
{
  if(a>=0)
  {
     analogWrite(5,a);
     digitalWrite(9,0);
  }
  else
  {
     analogWrite(9,-a);
     digitalWrite(5,0);
  }
  
  if(b>=0)
  {
     analogWrite(3,b);
     digitalWrite(6,0);
  }
  else
  {
     analogWrite(6,-b);
     digitalWrite(3,0);
  }
}
//functions for appropriate turns
void ru_turn(bool dd)
{
  if(dd==1) 
      gaadirun(0,-150);
  else if((left_turn==0)&&(diag_left==0))
      gaadirun(70,-100);
   else gaadirun(100,-100);
  while(1)
  {
     read_values();
     if(sensorval[3]==1&&sensorval[2]==1 ) 
        break;
  }
  while(1)
  {
    read_values();
    if(fsensorval==1) break;
  }
  while(1)
 {
     read_values();
     if(sensorval[1]==1 ) 
       break;
  } 
  state_enc=digitalRead(13);
}

void turn_right(bool dd)
{
  if(dd==1)
      gaadirun(0,-150);
  else  gaadirun(100,-100);
  while(1)
  {
    read_values();
    if(rsensorval==1) 
      break;
  }
  while(1)
  {
     read_values();
     if(sensorval[3]==1&&sensorval[2]==1 ) 
      break;
  }
  while(1)
  {
      read_values();
      if(fsensorval==1)
        break;
  }
  while(1)
  {
      read_values();
      if( sensorval[3]==0)
        break;
  }
   state_enc=digitalRead(13);
  //return;
}
void lu_turn(bool dd)
{
 if(dd) 
    gaadirun(-150,60);
 else if(dd==0)
 {
   if((right_turn==1)||(diag_right==1))
   {
    gaadirun(-100,100);
   }
   else gaadirun(-100,75);
 }
 while(1)
 {
     read_values();
     if(sensorval[0]==1&&sensorval[1]==1 ) 
      break;
 }
 while(1)
 {
  read_values();
  if(fsensorval==1) break;
 }
 while(1)
 {
     read_values();
     if(sensorval[2]==1 ) 
      break; 
 }
  state_enc=digitalRead(13);
}
void turn_left(bool dd)
{
  if(dd) 
      gaadirun(-150,0);
  else gaadirun(-100,100 );
  while(1)
  {
    read_values();
    if(lsensorval==1) 
      break;
  }
  while(1)
  {
        read_values();
        if(sensorval[1]==1 && sensorval[0]==1)
          break;
  }
  while(1)
  {
      read_values();
      if(fsensorval==1 )
        break;
  }
  while(1)
  {
      read_values();
      if(  sensorval[0]==0 )
        break;
  }
   state_enc=digitalRead(13);   
} 
void u_turn()
{
  gaadirun(120,-120);
   while(1)
  {
     read_values();
     if(rsensorval==1 ) 
        break;
  }
  while(1)
  {
    read_values();
    if(sensorval[2]==1) break;
  }

  state_enc=digitalRead(13);
}
void init_node()
{
  junctions[nodes_discovered-1].x=x3_coord;
  junctions[nodes_discovered-1].y=y3_coord;
  junctions[nodes_discovered-1].dir[(ang/45)%8]=straight;
  junctions[nodes_discovered-1].dir[(ang/45+1)%8]=diag_right;
  junctions[nodes_discovered-1].dir[(ang/45+2)%8]=right_turn;
  junctions[nodes_discovered-1].dir[(ang/45+4)%8]=1;
  junctions[nodes_discovered-1].dir[(ang/45+6)%8]=left_turn;
  junctions[nodes_discovered-1].dir[(ang/45+7)%8]=diag_left;
}
bool not_visited()
{
   double min_dist=100000000,temp;
   int min_node=-1;
   bool flag=0;
  if (upast==1)
      {
        return 0;
      }
  for(int i=0;i<nodes_discovered;i++)
  {
    if ((i==(curr_node-1))&&(tpast==0)){
      Serial.print("continue");Serial.println();
      continue;
    }
 
         temp=(junctions[i].x-x3_coord)*(junctions[i].x-x3_coord)+(junctions[i].y-y3_coord)*(junctions[i].y-y3_coord);
         if(temp<=85)
         {
         if(exor(junctions[i].dir[(ang/45)%8],straight)) continue;
            if( exor(junctions[i].dir[(ang/45+1)%8],diag_right))continue;
            if(exor(junctions[i].dir[(ang/45+2)%8],right_turn)) continue;
            if(junctions[i].dir[(ang/45+4)%8]==0) continue;
            if(exor(junctions[i].dir[(ang/45+6)%8],left_turn))continue;
            if(exor(junctions[i].dir[(ang/45+7)%8],diag_left)) continue;
             if(temp<=min_dist)
             {
              min_dist=temp;
              min_node=i;
             }
            //Serial.print("ALREADY VISITED");
            //Serial.print("min_node=");Serial.println(min_node);
            //Serial.print("min_dist=");Serial.println(min_dist);
        }
  }
  if(min_node>-1)
  {
   curr_node=min_node+1;
   new_node=0;
   return 0; 
  }
  else
  {
    new_node=1;
    return 1;
  }
  
}
//functions to choose which turn to take next(the crux of the algorithm)
char choose_turn()
{
if(new_node==0)
{
 if(junctions[curr_node-1].dir[(ang/45+4)%8]==2)
  {
    if (diag_right==1||right_turn==1)
    {
      ru_turn(0);
      gaadirun(0,0);
      delay(100);
      special_turn();
    }
    else if(straight!=0)turn_right(0);
    else u_turn();
    return 'p';
  }
  else if(junctions[curr_node-1].dir[(ang/45+4)%8]==3) 
  {
  if(junctions[curr_node-1].dir[(ang/45+6)%8]==1||junctions[curr_node-1].dir[(ang/45+7)%8]==1) return 'l';
  else if(junctions[curr_node-1].dir[(ang/45)%8]==1) return 's';
  else if(junctions[curr_node-1].dir[(ang/45+1)%8]==1||junctions[curr_node-1].dir[(ang/45+2)%8]==1) return 'r';
  else  if(junctions[curr_node-1].dir[(ang/45+6)%8]==2||junctions[curr_node-1].dir[(ang/45+7)%8]==2) return 'l';
  else if(junctions[curr_node-1].dir[(ang/45)%8]==2) return 's';
  else if(junctions[curr_node-1].dir[(ang/45+1)%8]==2||junctions[curr_node-1].dir[(ang/45+2)%8]==2) return 'r';
  else
    { 
      Serial.print("END");
      Serial.println();
    }
  }
} 
else 
  {
   if(right_turn==1||diag_right==1) return 'r';
   else if(straight==1) return 's';
  else if(left_turn==1||diag_left==1) return 'l';
  }
}
bool exor(int a, bool b)
{
  return (a*(!b)+b*(!a));
}
char get_next_direction()
{
  while(1)
  {
  curr_node++;
  if(junctions[curr_node-1].dir[ang/45]==2) return 's'; 
  else if(junctions[curr_node-1].dir[ang/45+1]==2||junctions[curr_node-1].dir[ang/45+2]==2) return 'r';
  else if(junctions[curr_node-1].dir[ang/45+6]==2||junctions[curr_node-1].dir[ang/45+7]==2) return 'l';
  
  }
}
void special_turn()
{
  gaadirun(80,-100);
  while(1)
  {
    read_values();
    if(rsensorval==0) 
      break;
  }
  while(1)
  {
    read_values();
    if(rsensorval==1) 
      break;
  }
  
  while(1)
  {
     read_values();
     if(sensorval[3]==1 ) 
      break;
  }
  while(1)
  {
      read_values();
      if(fsensorval==1)
        break;
  }
 
   state_enc=digitalRead(13); 
}

