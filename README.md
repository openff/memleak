# memleak
版本1
#mtrace使用
安装mtarce库 
sudo apt-get install libc6-dev
cd mtrace 
lua make.lua
./m5
./logri         //只输出内存泄露
addr:[0x111111] + 0xb817f0 0x14  falg:2
addr:[0x222222] + 0xb817a0 0x14  falg:2

addr2line -f -e  ./m5  -a  0x222222  0x111111 0x4011f9

./imitate   仿写mtrace原理



版本二 劫持malloc
lua make.lua
/mem/0x222222

addr2line -f -e  ./m2  -a  0x222222