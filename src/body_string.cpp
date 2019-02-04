/*!
 * httb.
 * body_string.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "httb/body_string.h"

httb::body_string::body_string(const std::string &body): m_body(body) {
}
httb::body_string::body_string(std::string &&body): m_body(std::move(body)) {
}

httb::body_string::~body_string() {
}
const std::string httb::body_string::build(httb::io_container *request) const {
    return m_body;
}
 
 