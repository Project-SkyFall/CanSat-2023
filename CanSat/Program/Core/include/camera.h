#ifndef CAMERA_H
#define CAMERA_H

class Camera{
    public:
    bool setup(bool verbose=false);
    void toggleOnOff(bool mode);
    void toggleRec();
    void seekLaunch();

    void printStatus();

    enum class Status{status_OFF = 0, status_REC = 1, status_ON = 2};
    Status status;
};

extern Camera cam;

#endif