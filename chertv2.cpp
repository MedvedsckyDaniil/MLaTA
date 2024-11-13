/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <vector>
std::string reduction(std::string str){
    if (str.find("|") != std::string::npos){
        int num = str.find("|");
        std::string str1 = str.substr(0, num);
        std::string str2 = str.substr(num+1, str.size()-1);
        if (str1==str2){
            str=str1;
        }
        if(str1.find("!") != std::string::npos && str2.find("!") == std::string::npos){
            str1 = str1.substr(str1.find("!"), str1.size()-1);
            if (str1==str2){
                str="@";
            }
        }
        if(str2.find("!") != std::string::npos && str1.find("!") == std::string::npos){
            str2 = str1.substr(str2.find("!"), str2.size()-1);
            if (str1==str2){
                str="□";
            }
        }
    }
    return str;
}

int is_in(std::vector<std::string> vec,std::string str){
    for (int i = 0; i< vec.size();i++){
        if (vec[i] == str){
            return 1;
        }
    }
    return 0;
}

std::string delete_or(std::string str1){
    while (str1.find("|")!= std::string::npos){
        str1.erase(str1.find("|"));
    }
    return str1;
}

      
std::vector<std::string> resolve(std::vector<std::string> vec, int i, int num, int j, int numj){
    std::string str1 = vec[i];
    std::string str2 = vec[j];
    str1.erase(num-1,3);
    str2.erase(numj,1);
    
    str1 = delete_or(str1);
    str2 = delete_or(str2);
    
    if (str2.length()>0){
        str1+="|";
        str1+=str2;
        //str1.erase(str1.end()-1);
    }

    //str1.erase(str1.begin());
    if (!is_in(vec, str1)){
        str1=reduction(str1);
        vec.push_back(str1);
    }
    return vec;
}      
       
std::string imlication(std::string str){
    for (int i =0; i<str.length();i++){
        if (str[i]=='>'){
            str[i]='|';
            str=str.insert(i-1, "!");
        }
    } 
    return str;
}

int main()
{
    std::vector<std::string> vec={"l|a","a>l","a|!l"};

    for (int i=0; i < vec.size();i++){
        if (vec[i].find(">") != std::string::npos){
            vec[i]= imlication(vec[i]);
        }
    }
int itter=vec.size();
int inter=0;
while (inter<itter){
    itter=vec.size();
    inter++;
    for (int i=0; i < vec.size();i++){
        if (vec[i].find("!") != std::string::npos){
            for (int j=0; j < vec.size();j++){
                int num = vec[i].find("!")+1;
                int numj = vec[j].find(vec[i][num]);
                if (numj == 0) {
                    //std::cout<<vec[i] <<" "<< vec[j] << "]]]]"<< std::endl;//true
                    vec=resolve(vec, i, num, j, numj);
                    inter++;
                    //std::cout<<vec.back()<<"]]]"<< std::endl;//true
                }
                else{
                    if (numj != std::string::npos && vec[j][numj-1]!='!'){
                        //std::cout<<vec[i] <<" "<< vec[j] << "ffff"<< std::endl;//true
                        vec=resolve(vec, i, num, j, numj);
                        //std::cout<<vec.back()<<"ffff"<< std::endl;//true
                        inter++;
                    }
                }
            }
            if(is_in(vec, "□")){
                std::cout << vec.size() << std::endl;
                for (int i=0; i<vec.size();i++){
                    std::cout<<vec[i] << " ";
                }
                std::cout<< std::endl;
                std::cout<<"thats all"<< std::endl;//true
                return 0;
            }
        }
    }
}
    std::cout << vec.size() << std::endl;
    for (int i=0; i<vec.size();i++){
        std::cout<<vec[i] << " ";
    }
    

    return 0;
}