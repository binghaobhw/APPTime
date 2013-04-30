#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

inline std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

int main()
{
    //we uses \\ instead of \ ( \ is a escape character ) in this string
    while(true) {
        cout << exec("xprop -id $(xprop -root | awk '/_NET_ACTIVE_WINDOW\\(WINDOW\\)/{print $NF}') | awk '/_NET_WM_PID\\(CARDINAL\\)/{print $NF}'"); 
        sleep(1);
    }
    return 0;
}
