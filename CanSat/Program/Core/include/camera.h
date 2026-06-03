#ifndef CAMERA_H
#define CAMERA_H

#define SM_A_PIN 9
#define DUMMY_PIN 5

class Camera{
    public:
    bool setup(bool verbose=false);
    void toggleOnOff(bool mode);
    void toggleRec();
    void seekLaunch();
    void smartAudioWrite(uint8_t command, uint8_t data);
    uint8_t smartAudioRead();

    void printStatus();

    enum class Status{status_OFF = 0, status_REC = 1, status_ON = 2};
    Status status;

    private:
    uint8_t crc8(uint8_t ptr[], uint8_t len);
};

extern Camera cam;

#endif