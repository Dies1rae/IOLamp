#ifndef DEVICE_BCKLIGHT_H_INCLUDED
#define DEVICE_BCKLIGHT_H_INCLUDED

#define STARTSYM 0xA5
#define DEFLEN 0x00
#define DEFCOLORLEN 0x03
#define SETFIRSTCOLORCMD 0x02
#define SETSECONDCOLORCMD 0x04
#define AWAITTIMEREAD 500
#define AWAITTIMEWRITE 50
#define LAMPOFF 0x00
#define LAMPRESET 0x02
#define LAMPON 0x01
#define WDT 0x07
#define DEFWDTLEN 0x01

//this define get C data array and fill is with element of templated type 0
#define ZERO_ANY(T, a, n) do {\
   T *a_ = (a);\
   size_t n_ = (n);\
   for (; n_ > 0; --n_, ++a_)\
     *a_ = (T) { 0x00 };\
} while (0)
#define ARRAY_SIZE(a) (sizeof (a) / sizeof *(a))
#define ZERO_ANY_A(T, a) ZERO_ANY(T, (a), ARRAY_SIZE(a))

#include <QSerialPort>
#include <QIODevice>
#include <iostream>

namespace IODEV {
    class DeviceBckLight {
    public:
        DeviceBckLight(char* port);
        //DeviceBckLight(const config::TMSService::DeviceBckLight* _drv);
        ~DeviceBckLight();

        int setColor(uint8_t R = 230, uint8_t G = 230, uint8_t B = 250, uint8_t lamp_num = 1);
        int setBitArray(uint8_t len);
    private:
        //Answer state machine
        enum AnswerCode {
            OK = 0,
            ERROR = 1,
            COMM_ERR = 99,
        };

        void clearBuffers();                                                        //чистака буфера с верхним дефайном
        uint8_t calculateCRC(const int buffer_len);                                 //посчитать CRC
        bool connect();                                                             //подключить порт
        void disconnect();                                                          //отстыковать порт
        DeviceBckLight::AnswerCode converse(const int buffer_len);                  //передать сообщение
        void prepareRequest(uint8_t R, uint8_t G, uint8_t B, uint8_t lamp_num);     //заполнение буфера




        bool connected = false;
        QSerialPort    m_port;
        char*          serial_port;                                                 //временный адрес порта
        uint8_t        buffer[20];                                                   //буфер данных
        uint8_t        buffer_read[20];                                                   //буфер данных
        //const config::IODEVService::DeviceBckLight* m_cfg;                        //настройки порта из oql
    };
} //namespace IODEV

#endif //DEVICE_BCKLIGHT_H_INCLUDED
