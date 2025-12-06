#include <SipStack.hpp>

#include <thread>

int main()
{
    try
    {
        SipStack::createAndInitInstance();
        SipStack* sipStack = SipStack::getInstance();

        // Keep the application running to process SIP messages
        while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}