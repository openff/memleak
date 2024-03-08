#include <iostream>
#include <fstream>
#include <string>
#include  <unordered_map>
struct DATA
{
    std::string data;
    int flag;
};

int main() {
    std::string filePath = "./test.log";
    std::string line;
    std::ifstream file(filePath);
    std::unordered_map<std::string,DATA> map;
    if (!file.is_open()) {
        std::cerr << "无法打开文件：" << filePath << std::endl;
        return 1;
    }

    /*
        = Start
        @ ./memleak4:[0x40061d] + 0x1f9e450 0xa  22
        @ ./memleak4:[0x40062b] + 0x1f9e470 0x14
        @ ./memleak4:[0x40063b] - 0x1f9e450
        @ ./memleak4:[0x400645] + 0x1f9e450 0x14
        @ ./memleak4:[0x400653] + 0x1f9e490 0x14
        @ ./memleak4:[0x400663] - 0x1f9e470
        @ ./memleak4:[0x40066f] - 0x1f9e490
        = End

    = Start
    @ ./memleak4:(main+0x1b)[0x4011d1] + 0xea6750 0xa
    @ ./memleak4:(main+0x29)[0x4011df] + 0xea67a0 0x14
    @ ./memleak4:(main+0x39)[0x4011ef] - 0xea6750
    @ ./memleak4:(main+0x43)[0x4011f9] + 0xea6750 0x14
    @ ./memleak4:(main+0x51)[0x401207] + 0xea67f0 0x14
    @ ./memleak4:(main+0x61)[0x401217] - 0xea67a0
    @ ./memleak4:(main+0x6d)[0x401223] - 0xea67f0
    @ ./memleak4:(main+0x77)[0x40122d] + 0xea67f0 0x14
    @ ./memleak4:(main+0x85)[0x40123b] + 0xea67a0 0x14
    = End
    */
    while (std::getline(file, line)) {
        
        //std::cout <<line<< std::endl;
        auto target = line.find("[",0);
        if (target != std::string::npos)
        {  
            //[0x40063b] - 0x1f9e450 [0x401207] + 0xea67f0
            auto rigth = line.find("]",target);
            std::string values  = line.substr(target,rigth);
            std::string key  = line.substr(rigth,11);
            int fh=0;
            auto it = map.find(key);
            if(it != map.end() )
            {
                 //相同
                if (line.at(rigth+2) == '+')
                {
                    (*it).second.flag+=1;
                    (*it).second.data = values;
                    
                }else{
                   (*it).second.flag -=1;
                }
               
            }else{
                //插入
                if (line.at(rigth+2) == '+')
                {
                    fh+=1;
                }else{
                    fh-=1;
                }
                DATA data{values,fh};
                map.insert({key,data});
            }
           // std::cout << q <<"  "<<fh<<"  "<< key <<std::endl;
         
           
        }
        
       
    }
    
    for ( auto& data : map)
    {
        if (data.second.flag >  0)
        {
              std::cout << "addr:" <<data.second.data  <<"  falg:"<<data.second.flag<<std::endl;
        }
        //std::cout << data.first <<"  "<<data.second.flag<<"  "<< data.second.data <<std::endl;
    }
    
    file.close();
     std::cout << "end"<<std::endl;
    return 0;
}