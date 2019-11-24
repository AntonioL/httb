/*!
 * httb.
 * body.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef HTTB_BODY_H
#define HTTB_BODY_H

#include <string>
#include "httb/io_container.h"

namespace httb {

class request_body {
 public:
    virtual ~request_body() = default;

    virtual std::string build(httb::io_container *request) const = 0;

};

}

#endif //HTTB_BODY_H
