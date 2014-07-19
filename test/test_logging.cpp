#include "gdrive/logging.hpp"

using namespace GDRIVE;
class LoggingTest {
    CLASS_MAKE_LOGGER
    public:
        LoggingTest() {
            CLASS_INIT_LOGGER("LoggingTest", DEBUG)
        }

        void testClassLogger() {
            CLOG_DEBUG("This is a debug information\n");
            CLOG_INFO("This is a info information\n");
            CLOG_WARN("This is a warn information\n");
            //CLOG_ERROR("This is a error information\n");
        }

        void testFunctionLogger() {
            FUNC_MAKE_LOGGER
            FUNC_LOGGER_SET_LEVEL(DEBUG);
            FLOG_DEBUG("This is a debug information\n");
            FLOG_INFO("This is a info information\n");
            FLOG_WARN("This is a warn information\n");
            FLOG_ERROR("This is a error information\n");
        }
};

int main() {
    LoggingTest test;
    test.testClassLogger();
    test.testFunctionLogger();
}
