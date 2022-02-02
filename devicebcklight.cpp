#include "devicebcklight.h"
#include <QTextStream>
#include <QDebug>

using namespace IODEV;

DeviceBckLight::DeviceBckLight(char* port) : serial_port(port) {
    try {
        this->connect();
        //this->bcklightOn();
    } catch (...) {
        std::cout << "Can't open port, need to reconnect\n";
    }
}

DeviceBckLight::~DeviceBckLight() {
    this->disconnect();
}

bool DeviceBckLight::connect() {
    this->m_port.setPortName(this->serial_port);
    this->m_port.setBaudRate(QSerialPort::Baud57600);
    this->m_port.setDataBits(QSerialPort::Data8);
    this->m_port.setStopBits(QSerialPort::OneStop);
    this->m_port.setFlowControl(QSerialPort::NoFlowControl);
    this->connected = this->m_port.open(QIODevice::ReadWrite);
    qDebug() << this->m_port.error();
    return this->connected;
}

void DeviceBckLight::disconnect() {
    this->connected = false;
    this->m_port.close();
}

uint8_t DeviceBckLight::calculateCRC(const int buffer_len) {
    uint8_t crc_answ = 0;
    for(int ptr = 0; ptr < buffer_len; ptr++) {
        crc_answ += this->buffer[ptr];
    }
    return 0x100 - crc_answ;
}

int DeviceBckLight::setColor(uint8_t R, uint8_t G, uint8_t B, uint8_t lamp_num) {
    if(!this->connected) {
        this->connect();
        std::cout << "Can't connect, try to reconnect\n";
        return DeviceBckLight::AnswerCode::COMM_ERR;
    }

    switch (lamp_num)
    {
    case 1:
        this->prepareRequest(R, G, B, lamp_num);
        return this->converse(9);
        break;
    case 2:
        this->prepareRequest(R, G, B, lamp_num);
        return this->converse(9);
        break;
    default:
        return DeviceBckLight::AnswerCode::ERROR;
        break;
    }
    return DeviceBckLight::AnswerCode::ERROR;
}

DeviceBckLight::AnswerCode DeviceBckLight::converse(const int buffer_len) {
    if(!this->connected) {
        this->connect();
    }
    QByteArray wrbuff;
    for(int ptr = 0; ptr < buffer_len; ptr++) {
        wrbuff.append(this->buffer[ptr]);
    }

    this->m_port.waitForBytesWritten(AWAITTIMEWRITE);
    this->m_port.write(wrbuff, buffer_len);
    this->m_port.waitForReadyRead(AWAITTIMEREAD);
    wrbuff.clear();
    this->clearBuffers();
    wrbuff = this->m_port.readAll();

    for(int ptr = 0; ptr < wrbuff.size(); ptr++) {
        this->buffer_read[ptr] = wrbuff.at(ptr);
    }

    return DeviceBckLight::AnswerCode::OK;
}


void DeviceBckLight::prepareRequest(uint8_t R, uint8_t G, uint8_t B, uint8_t lamp_num) {
    this->buffer[0] = STARTSYM;
    this->buffer[1] = DEFLEN;
    this->buffer[2] = lamp_num * 2;
    this->buffer[3] = DEFLEN;
    this->buffer[4] = DEFCOLORLEN;
    this->buffer[5] = R;
    this->buffer[6] = G;
    this->buffer[7] = B;
    this->buffer[8] = this->calculateCRC(8);
}

void DeviceBckLight::clearBuffers() {
    ZERO_ANY(uint8_t, this->buffer, 11);
    ZERO_ANY(uint8_t, this->buffer_read, 11);
}

int DeviceBckLight::setBitArray(uint8_t len) {
    QTextStream s(stdin);
    for(int ptr = 0; ptr < len; ptr++) {
        std::cout << "set " << ptr << " byte: \n";
        QString value = s.readLine();
        this->buffer[ptr] = value.toUInt(nullptr, 10);
    }
    this->buffer[len - 1] = this->calculateCRC(len - 1);

    std::cout << "Buffer setted as byte:\n";
    for(int ptr = 0; ptr < len; ptr++) {
        std::cout << this->buffer[ptr] << ' ';
    }
    std::cout << '\n';
    std::cout << "Buffer setted as hex:\n";
    for(int ptr = 0; ptr < len; ptr++) {
        printf("%02X ", this->buffer[ptr]);
    }
    std::cout << '\n';

    this->converse(len);

    this->disconnect();
    std::cout << "Buffer getted:\n";
    for(int ptr = 0; ptr < 11; ptr++) {
        printf("%02X ", this->buffer_read[ptr]);
    }
    std::cout << '\n';
    std::cout << std::endl;
    return 0;
}
