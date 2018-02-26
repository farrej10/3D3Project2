#include "routingtable.h"


routetable::routetable(char name){
    stable = false;
    ID = name;
    nodes = 0;
    char list [] = {'A','B','C','D','E','F'} ;
    string sample = "X,X,XXXXX";

    int i = 0;
    for(i ; i < 6 ; i++){
        if(list[i] != ID){
            sample[0] = list[i];
            RTentries.push_back(sample);
        }
        else{
            sample[0] = list[i];
            sample[2] = '0';
            RTentries.push_back(sample);
            sample[2] = 'X';
        }
    }

}

void routetable::start_up(){
    char buffer[12];
    FILE *f;
    bool stop = false;
    int i = 0;
    f = fopen("top.txt","r");
    if(f != NULL){
        while(!feof(f)){
            fgets(buffer,13,f);
            if(buffer[0] == ID){
                string a;
                a += buffer[2];
                a += buffer[3];
                a += buffer[10];
                a += buffer[3];
                a += buffer[4];
                a += buffer[5];
                a += buffer[6];
                a += buffer[7];
                a += buffer[8];
                //cout << a << endl;
                //
                for(i=0;i < RTentries.size(); i++){
                    if(RTentries[i][0] == a[0]){
                        RTentries[i] = a;
                        //cout<< "ok" << endl;
                    }
                }
            }
        }
    }

}

void routetable::add_entry(string DV){
    int i;
    int j = 2;
    int est = 0;
    int dist = 0;
    char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    for(i=0 ; i < RTentries.size() ; i++){
         if(RTentries[i][0] == DV[0] && RTentries[i][2] != 'X'){
             est = (int)RTentries[i][2] - 48;
         }   
    }
    for(i=0 ; i < RTentries.size() ; i++){
        if(DV[j] != 'X'){
            dist =  ((int)DV[j] - 48);
            //cout << "Estimated Distance =" << dist << " " << RTentries[i][0] <<endl;
            if(dist < ((int)RTentries[i][2] - 48) && RTentries[i][2] != 'X'){
                RTentries[i][2] = digits[dist];
                // yet to append UDP for next hop
            }
            else if( RTentries[i][2] == 'X'){
                RTentries[i][2] = digits[dist];
                 // yet to append UDP for next hop
            }
        }   
        j += 2;
        
    }
}


