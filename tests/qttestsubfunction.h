#ifndef QT_TEST_SUB_FUNCTION_H
#define QT_TEST_SUB_FUNCTION_H

#define __SUB_TEST_FUNCTION_SUCESS_INDICATOR __mSuccess
#define HAS_SUB_TEST_FUNCTIONS bool __SUB_TEST_FUNCTION_SUCESS_INDICATOR;
#define BEGIN_SUB_TEST_FUNCTION __SUB_TEST_FUNCTION_SUCESS_INDICATOR = false;
#define END_SUB_TEST_FUNCTION __SUB_TEST_FUNCTION_SUCESS_INDICATOR = true;
#define SUB_TEST_FUNCTION(__fun__) \
    do { \
        __fun__; \
        if (!__SUB_TEST_FUNCTION_SUCESS_INDICATOR) \
            return; \
        else \
            __SUB_TEST_FUNCTION_SUCESS_INDICATOR = false; \
    } while(false)
    
#endif // QT_TEST_SUB_FUNCTION_H
