/*!
 * wsserver.
 * HttpClientTest.cpp
 *
 * \date 2018
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <httb/httb.h>
#include <boost/thread.hpp>
#include <fstream>
#include <toolboxpp.h>
#include "gtest/gtest.h"

TEST(HttpClientTest, TestBuildRequestSimple) {
    httb::request req("http://localhost:9000/simple-server.php/get");
    ASSERT_FALSE(req.isSSL());
    ASSERT_STREQ(req.getProto().c_str(), "http");
    ASSERT_EQ(req.getPort(), (uint16_t)9000);
    ASSERT_STREQ(req.getPortString().c_str(), "9000");
    ASSERT_STREQ(req.getHost().c_str(), "localhost");
    ASSERT_STREQ(req.getPath().c_str(), "/simple-server.php/get");
    ASSERT_STREQ(req.getParamsString().c_str(), "");
    ASSERT_STREQ(req.getUrl().c_str(), "http://localhost:9000/simple-server.php/get");
}

TEST(HttpClientTest, TestBuildRequestGoogleQuery) {
    const std::string src = "https://www.google.com/search?q=boost+beast&oq=boost+beast&aqs=chrome.0.69i59l3j69i60l3.2684j1j9&sourceid=chrome&ie=UTF-8";
    httb::request req(src);
    ASSERT_TRUE(req.isSSL());
    ASSERT_STREQ(req.getProto().c_str(), "https");
    ASSERT_EQ(req.getPort(), (uint16_t)443);
    ASSERT_STREQ(req.getPortString().c_str(), "443");
    ASSERT_STREQ(req.getHost().c_str(), "www.google.com");
    ASSERT_STREQ(req.getPath().c_str(), "/search");
    ASSERT_STREQ(req.getParamsString().c_str(), "?q=boost+beast&oq=boost+beast&aqs=chrome.0.69i59l3j69i60l3.2684j1j9&sourceid=chrome&ie=UTF-8");
    ASSERT_STREQ(req.getParam("q").c_str(), "boost+beast");
    ASSERT_STREQ(req.getParam("oq").c_str(), "boost+beast");
    ASSERT_STREQ(req.getParam("aqs").c_str(), "chrome.0.69i59l3j69i60l3.2684j1j9");
    ASSERT_STREQ(req.getParam("sourceid").c_str(), "chrome");
    ASSERT_STREQ(req.getParam("ie").c_str(), "UTF-8");


    ASSERT_STREQ(req.getUrl().c_str(), src.c_str());
}

TEST(HttpClientTest, TestSimpleGet) {
    httb::request req("http://localhost:9000/simple-server.php/get");
    httb::client client;
    client.setEnableVerbose(true);
    httb::response resp = client.execute(req);

    const std::string body = resp.getBody();
    if(!resp.isSuccess()) {
        std::cout << "Error response:" << std::endl;
        std::cout << body << std::endl;
    }
    ASSERT_TRUE(resp.isSuccess());
    ASSERT_STREQ(body.c_str(), "This is GET method response!");
}

TEST(HttpClientTest, TestSimplePost) {
    httb::request req("http://localhost:9000/simple-server.php/post");
    req.setMethod(httb::request::method::post);
    req.setBody(httb::body_form_urlencoded("aaa=1&bbb=2&ccc=3"));
    req.setHeader({"content-type", "application/x-www-form-urlencoded"});

    httb::client client;
    client.setEnableVerbose(true);
    httb::response resp = client.execute(req);

    if(!resp.isSuccess()) {
        std::cout << "Error response:" << std::endl;
        std::cout << resp.getBody() << std::endl;
    }
    ASSERT_TRUE(resp.isSuccess());
    ASSERT_STREQ(resp.getBodyC(), "This is POST method response!");
}

TEST(HttpClientTest, TestSimplePostSmallFile) {
    httb::request req("http://localhost:9000/simple-server.php/file");
    req.setMethod(httb::request::method::post);

    httb::body_multipart body;

    body.addEntry({"myfile", "text/plain", "myfile.txt", toolboxpp::fs::readFile(std::string(TEST_ROOT) + "/mock/test.txt")});
    body.addEntry({"somekey", "somevalue"});

    req.setBody(body);

    httb::client client;
    client.setEnableVerbose(true);
    httb::response resp = client.execute(req);

    if(!resp.isSuccess()) {
        std::cout << "Error response:" << std::endl;
        std::cout << resp.getBody() << std::endl;
    }
    ASSERT_TRUE(resp.isSuccess());
    ASSERT_STREQ(resp.getBodyC(), "This is POST method response!");
}

TEST(HttpClientTest, TestSimplePostMediumFile) {
    httb::request req("http://localhost:9000/simple-server.php/file");
    req.setMethod(httb::request::method::post);

    httb::body_multipart body;

    body.addEntry({"myfile", "application/binary", "my-medium-file.txt", toolboxpp::fs::readFile(std::string(TEST_ROOT) + "/mock/test_medium.bin")});
    body.addEntry({"somekey", "somevalue"});

    req.setBody(body);

    httb::client client;
    client.setEnableVerbose(true);
    httb::response resp = client.execute(req);

    if(!resp.isSuccess()) {
        std::cout << "Error response:" << std::endl;
        std::cout << resp.getBody() << std::endl;
    }
    ASSERT_TRUE(resp.isSuccess());
    ASSERT_STREQ(resp.getBodyC(), "This is POST method response!");
}

TEST(HttpClientTest, TestSimplePostBigFile) {
    httb::request req("http://localhost:9000/simple-server.php/file");
    req.setMethod(httb::request::method::post);

    httb::body_multipart body;

    body.addEntry({"myfile", "application/binary", "my-big-file.txt", toolboxpp::fs::readFile(std::string(TEST_ROOT) + "/mock/test_big.bin")});
    body.addEntry({"somekey", "somevalue"});

    req.setBody(body);

    httb::client client;
    client.setEnableVerbose(true);
    httb::response resp = client.execute(req);

    if(!resp.isSuccess()) {
        std::cout << "Error response:" << std::endl;
        std::cout << resp.getBody() << std::endl;
    }
    ASSERT_TRUE(resp.isSuccess());
    ASSERT_STREQ(resp.getBodyC(), "This is POST method response!");
}

TEST(HttpClientTest, TestSimplePut) {
    httb::request req("http://localhost:9000/simple-server.php/put");
    req.setMethod(httb::request::method::put);
    req.setBody("aaa=1&bbb=2&ccc=3");
    req.setHeader({"content-type", "application/x-www-form-urlencoded"});

    httb::client client;
    client.setEnableVerbose(true);
    httb::response resp = client.execute(req);

    if(!resp.isSuccess()) {
        std::cout << "Error response:" << std::endl;
        std::cout << resp.getBody() << std::endl;
    }
    ASSERT_TRUE(resp.isSuccess());
    ASSERT_STREQ(resp.getBodyC(), "This is PUT method response!");
}

TEST(HttpClientTest, TestSimpleDelete) {
    httb::request req("http://localhost:9000/simple-server.php/delete");
    req.setMethod(httb::request::method::delete_);

    httb::client client;
    client.setEnableVerbose(true);
    httb::response resp = client.execute(req);

    if(!resp.isSuccess()) {
        std::cout << "Error response:" << std::endl;
        std::cout << resp.getBody() << std::endl;
    }
    ASSERT_TRUE(resp.isSuccess());
    ASSERT_STREQ(resp.getBodyC(), "This is DELETE method response!");
}

TEST(HttpClientTest, TestGetWithRedirectAndDisabledFollow) {
    httb::request req("http://www.boost.org/doc/libs/develop/libs/beast/doc/html/beast/using_http/message_containers.html");
    httb::client client;
    httb::response resp = client.execute(req);

    ASSERT_TRUE(resp.isSuccess());
    ASSERT_EQ(resp.statusCode, 301);
}

TEST(HttpClientTest, TestGetWithRedirectAndEnabledFollow) {
    httb::request req("http://www.boost.org/doc/libs/develop/libs/beast/doc/html/beast/using_http/message_containers.html");
    httb::client client;
    client.setFollowRedirects(true);
    httb::response resp = client.execute(req);

    ASSERT_TRUE(resp.isSuccess());
    ASSERT_EQ(resp.statusCode, 200);
}

TEST(HttpClientTest, TestReusingClientInstance) {
    httb::request req("https://www.boost.org/doc/libs/develop/libs/beast/doc/html/beast/using_http/message_containers.html");
    httb::client client;
    client.setFollowRedirects(true);
    httb::response resp1 = client.execute(req);
    ASSERT_TRUE(resp1.isSuccess());
    ASSERT_EQ(resp1.statusCode, 200);

    httb::response resp2 = client.execute(req);
    ASSERT_TRUE(resp2.isSuccess());
    ASSERT_EQ(resp2.statusCode, 200);
}

TEST(HttpClientTest, TestSimpleAsyncGet) {
    httb::request req("http://localhost:9000/simple-server.php/get");
    httb::request req2("http://localhost:9000/simple-server.php/get");
    httb::client client;
    client.setEnableVerbose(true);
    bool executed1,executed2 = false;
    bool responseIsSuccess1, responseIsSuccess2 = false;
    client.executeAsync(req, [&executed1, &responseIsSuccess1](httb::response response) {
      executed1 = true;
      responseIsSuccess1 = response.isSuccess();
      ASSERT_STREQ(response.getBodyC(), "This is GET method response!");
    });
    client.executeAsync(req2, [&executed2, &responseIsSuccess2](httb::response response) {
      executed2 = true;
      responseIsSuccess2 = response.isSuccess();
      ASSERT_STREQ(response.getBodyC(), "This is GET method response!");
    });

    int tries = 0;
    while(!executed1) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        tries++;
        if(tries >= 3) {
            executed1 = false;
            break;
        }
    }

    tries = 0;
    while(!executed2) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        tries++;
        if(tries >= 3) {
            executed2 = false;
            break;
        }
    }

    ASSERT_TRUE(executed1);
    ASSERT_TRUE(executed2);
    ASSERT_TRUE(responseIsSuccess1);
    ASSERT_TRUE(responseIsSuccess2);
}

//TEST(HttpClientTest, TestAsyncClient) {
//    httb::request req("http://localhost:9000/simple-server.php/get");
//    std::shared_ptr<httb::client_async> client = std::make_shared<httb::client_async>();
//    client->execute(req, [](boost::system::error_code ec, httb::response resp) {
//
//    });
//
//}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char** argv) {
    std::stringstream ss1;
    ss1 << TEST_ROOT << "/mock/run-server.sh " << TEST_ROOT << "/mock";

    std::cout << ss1.str() << std::endl;
    const std::string pid = exec(ss1.str().c_str());


    ::testing::InitGoogleTest(&argc, argv);

    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    int ret = RUN_ALL_TESTS();

    std::stringstream ss;
    ss << "kill -9 " << pid;
    system(ss.str().c_str());

    return ret;
}


 