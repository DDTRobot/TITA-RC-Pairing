#include "uart-crsf.h"
#include "uart.h"
#include <string.h>
#include <cstdio>


CRSF::CRSF()
{
    m_disconnected = true;
}

CRSF::~CRSF()
{

}

void CRSF::begin()
{
    uint8 loc_crsfData[CRSF_PACKET_SIZE] = {0x00};
    sint16 loc_channels[16] = {1023};

    memcpy(m_crsfData, loc_crsfData, CRSF_PACKET_SIZE);
    memcpy(m_channels, loc_channels, CRSF_MAX_CHANNEL);
}


void CRSF::customer_data_callback(const uint8_t data[])
{
    switch (data[0]) {
    case CRSF_CUSTOMER_CMD_DISCONNECT: {
    m_disconnected = TRUE;
    } break;

    case CRSF_CUSTOMER_CMD_IS_BINGDING: {
    } break;

    case CRSF_CUSTOMER_CMD_UART_LINKED: {
    } break;

    default:
    break;
    }
}

void CRSF::readPacket(uint8_t* inData,uint8_t len)
{
    uint8_t frameLength, bufferIndex = 0;

#if 0
    if (inData[0] == CRSF_CUSTOMER_CMD) {
      if (inData[1] == CRSF_CUSTOMER_CMD_REV_DATA) {
        customer_data_callback(&inData[2]);
        return;
      }
    }
#endif

    if (bufferIndex == 0)
    {
        if (inData[0] == CRSF_ADDRESS_FLIGHT_CONTROLLER)
        {
            m_inBuffer[bufferIndex++] = inData[0];
            frameLength = inData[1];
            m_inBuffer[bufferIndex++] = inData[1];
        }
        else
        {
            bufferIndex = 0;
        }
    }

    if (bufferIndex > 1 && bufferIndex < frameLength + 1)
    {
        for(uint8_t i = bufferIndex; i < frameLength + 1; i++)
        {
            m_inBuffer[bufferIndex++] = inData[i];
        }
    }

    if (bufferIndex == frameLength + 1)
    {
        m_inBuffer[bufferIndex++] = inData[25];

        uint8_t crc = crsf_crc8(&m_inBuffer[2], m_inBuffer[1] - 1);

        m_inBuffer[24] = crc;

        if (frameLength == CRSF_FRAME_LENGTH && m_inBuffer[0] == CRSF_ADDRESS_FLIGHT_CONTROLLER)
        {
            if (crc == m_inBuffer[25])
            {
                memcpy(m_crsfData, m_inBuffer, CRSF_PACKET_SIZE);
                m_disconnected = false;
                updateChannels();
            }
            else
            {
            }
        }
    }
}

uint16 CRSF::getChannel(uint8 channel) const
{
    return m_channels[channel - 1];
}

void CRSF::updateChannels()
{
    if (m_crsfData[1] == 24)
    {
        m_channels[0] = ((m_crsfData[3] | m_crsfData[4] << 8) & 0x07FF);
        m_channels[1] = ((m_crsfData[4] >> 3 | m_crsfData[5] << 5) & 0x07FF);
        m_channels[2] = ((m_crsfData[5] >> 6 | m_crsfData[6] << 2 | m_crsfData[7] << 10) & 0x07FF);
        m_channels[3] = ((m_crsfData[7] >> 1 | m_crsfData[8] << 7) & 0x07FF);
        m_channels[4] = ((m_crsfData[8] >> 4 | m_crsfData[9] << 4) & 0x07FF);
        m_channels[5] = ((m_crsfData[9] >> 7 | m_crsfData[10] << 1 | m_crsfData[11] << 9) & 0x07FF);
        m_channels[6] = ((m_crsfData[11] >> 2 | m_crsfData[12] << 6) & 0x07FF);
        m_channels[7] = ((m_crsfData[12] >> 5 | m_crsfData[13] << 3) & 0x07FF);
        m_channels[8] = ((m_crsfData[14] | m_crsfData[15] << 8) & 0x07FF);
        m_channels[9] = ((m_crsfData[15] >> 3 | m_crsfData[16] << 5) & 0x07FF);
        m_channels[10] = ((m_crsfData[16] >> 6 | m_crsfData[17] << 2 | m_crsfData[18] << 10) & 0x07FF);
        m_channels[11] = ((m_crsfData[18] >> 1 | m_crsfData[19] << 7) & 0x07FF);
        m_channels[12] = ((m_crsfData[19] >> 4 | m_crsfData[20] << 4) & 0x07FF);
        m_channels[13] = ((m_crsfData[20] >> 7 | m_crsfData[21] << 1 | m_crsfData[22] << 9) & 0x07FF);
        m_channels[14] = ((m_crsfData[22] >> 2 | m_crsfData[23] << 6) & 0x07FF);
        m_channels[15] = ((m_crsfData[23] >> 5 | m_crsfData[24] << 3) & 0x07FF);
    }

    m_dataReceivedCallback(m_channels);
}

uint8 CRSF::crsf_crc8(const uint8 *ptr, uint8 len) const
{
    static const uint8 crsf_crc8tab[256] = {
        0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
        0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
        0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
        0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
        0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
        0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
        0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
        0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
        0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
        0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
        0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
        0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
        0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
        0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
        0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
        0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9};

    uint8 crc = 0;
    for (uint8 i = 0; i < len; i++)
    {
        crc = crsf_crc8tab[crc ^ *ptr++];
    }
    return crc;
}

void CRSF::onDataReceived(void (*pCallback)(const uint16 channels[]))
{
    m_dataReceivedCallback = pCallback;
}

void CRSF::onDisconnected(void (*pCallback)())
{
    m_disconnectedCallback = pCallback;
}

bool CRSF::isConnected()
{
    return (!m_disconnected);
}

bool CRSF::isUartConnected()
{
    return (uart_connected);
}

void CRSF::setUartConnected(bool is_connected)
{
    uart_connected = is_connected;
}