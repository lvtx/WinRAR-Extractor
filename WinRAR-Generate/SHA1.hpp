/*********************************************************************
* Filename:   SHA1.hpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
*********************************************************************/
#include <stddef.h>
#include <stdint.h>

namespace accel::algorithm::hash::SHA1 {

    void Initialize(uint32_t hashResult[5]);
    void MainCycle(const uint8_t srcBytes[], size_t srcBytesLength, uint32_t hashResult[5]);
    void Final(const uint8_t srcBytes[], size_t srcBytesLength, uint32_t hashResult[5]);

}
