#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

//string brt_chars = "...,,,ooo000";
string brt_chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,^`\'. ";
//string brt_chars = ".:!";

char* getCmdOption(char** begin, char** end, const std::string & option){
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end){
        return *itr;
    }
    return 0;
}bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


string cvToString(Mat img, string chars){
    string ss;
    for(int i = 0; i < img.rows; i++){
        ss += '\n';
        for(int j = 0; j < img.cols; j++){
            Vec3b v = img.at<Vec3b>(i, j);
            int sorta_brt = (v[0] + v[1] + v[2]) / 3;

            int index = sorta_brt / (256 / chars.size());

            if(index >= chars.size()) index = chars.size() - 1;

            index = chars.size()-1 - index;

            ss += chars[index];
        }
    }
    return ss;
}

int main(int argc, char* argv[])
{
    int width = 200; int height = 60; int deviceID = 1; bool show = false; bool cam = true; bool file = false; char* filename = NULL; int fps = 30;

    if (cmdOptionExists(argv, argv+argc, "-w")){
        width = atoi(getCmdOption(argv, argv+argc, "-w"));
    }if (cmdOptionExists(argv, argv+argc, "-h")){
        height = atoi(getCmdOption(argv, argv+argc, "-h"));
    }if (cmdOptionExists(argv, argv+argc, "-d")){
        deviceID = atoi(getCmdOption(argv, argv+argc, "-d"));
    }if (cmdOptionExists(argv, argv+argc, "-s")){
        show = true;
    }if (cmdOptionExists(argv, argv+argc, "-f")){
        cam = false;
        file = true;
        filename = getCmdOption(argv, argv+argc, "-f");
    }if (cmdOptionExists(argv, argv+argc, "-fps")){
        fps = atoi(getCmdOption(argv, argv+argc, "-fps"));
    }

    Mat frame;
    VideoCapture cap;
     
    int apiID = cv::CAP_ANY;    

    if(cam){
        cap.open(deviceID, apiID);
    }else if(file){
        cap.open(filename);
    }

    if (!cap.isOpened()) {
        cerr << "unable to open cam\n";
        return -1;
    }

    for (;;)
    {

        cap.read(frame);

        resize(frame, frame, Size(width, height));
        //system("clear");

        cout << "\033[0;0H" << cvToString(frame, brt_chars);

        if(show){
            imshow("gec", frame);
            if (waitKey(5) >= 0)
                break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
    }

    return 0;
}
