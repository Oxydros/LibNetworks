//
// Created by Louis VENTRE (aka Oxydros) on 12/21/17.
//

#ifndef LIBNETWORKS_PACKETPARSER_H
#define LIBNETWORKS_PACKETPARSER_H
#include <memory>
#include <cassert>
#include <boost/circular_buffer.hpp>
#include "Common.h"
#include "IPacket.h"

namespace Network
{
    template<typename OutputPacket>
    std::unique_ptr<OutputPacket> extractPacketFromCircularBuffer(boost::circular_buffer<char> &buffer)
    {
        auto                                rangeOne{buffer.array_one()};
        auto                                rangeTwo{buffer.array_two()};
        std::size_t                         sizePacket{0};
        std::size_t                         copyFromOne{rangeOne.second >= HEADER_SIZE ? HEADER_SIZE : rangeOne.second};
        std::size_t                         copyFromTwo{rangeTwo.second >= (HEADER_SIZE - copyFromOne) ?
                                                        (HEADER_SIZE - copyFromOne) : rangeTwo.second};
        std::size_t                         saveCopyOne{copyFromOne};
        std::size_t                         saveCopyTwo{copyFromTwo};
        std::vector<unsigned char>          finalData{};
        std::unique_ptr<OutputPacket>       ptr{};

        if (!std::is_base_of<IPacket, OutputPacket>::value)
        {
            dout << "Parser: Template is not of type IPacket" << std::endl;
            assert(false);
        }

        dout << "Parser: Got two array: " << rangeOne.second << " and " << rangeTwo.second << std::endl;

        dout << "Parser: Extract size from array one(" << copyFromOne
             << ") and two(" << copyFromTwo << ")" << std::endl;

        if ((copyFromOne + copyFromTwo) != HEADER_SIZE)
            return (ptr);

        std::memcpy(&sizePacket, rangeOne.first, copyFromOne);
        std::memcpy(&sizePacket + copyFromOne, rangeTwo.first, copyFromTwo);

        dout << "Parser: Got packet of size " << sizePacket << std::endl;
        if (sizePacket == 0)
            return (ptr);

        copyFromOne = (rangeOne.second - copyFromOne) >= sizePacket ?
                      sizePacket : (rangeOne.second - copyFromOne);
        copyFromTwo = (rangeTwo.second - copyFromTwo) >= (sizePacket - copyFromOne) ?
                      (sizePacket - copyFromOne) : (rangeTwo.second - copyFromTwo);

        dout << "Parser: Extract packet from array one(" << copyFromOne
             << ") and two(" << copyFromTwo << ")" << std::endl;

        if ((copyFromOne + copyFromTwo) != sizePacket)
            return (ptr);
        finalData.resize(sizePacket);
        std::memcpy(finalData.data(), rangeOne.first + saveCopyOne, copyFromOne);
        std::memcpy(finalData.data() + copyFromOne, rangeTwo.first + saveCopyTwo, copyFromTwo);
        ptr = std::make_unique<OutputPacket>();
        static_cast<IPacket*>(ptr.get())->setData(finalData);
        dout << "Parser: Removing " << HEADER_SIZE + sizePacket << " from circular buffer" << std::endl;
        buffer.erase_begin(HEADER_SIZE + sizePacket);
        return (ptr);
    }
}

#endif //LIBNETWORKS_PACKETPARSER_H
