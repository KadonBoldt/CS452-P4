#include "harness/unity.h"
#include "../src/lab.h"

// NOTE: Due to the multi-threaded nature of this project. Unit testing for this
// project is limited. I have provided you with a command line tester in
// the file app/main.cp. Be aware that the examples below do not test the
// multi-threaded nature of the queue. You will need to use the command line
// tester to test the multi-threaded nature of your queue. Passing these tests
// does not mean your queue is correct. It just means that it can add and remove
// elements from the queue below the blocking threshold.


void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}




void test_create_destroy(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    queue_destroy(q);
}

void test_queue_dequeue(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    enqueue(q, &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    queue_destroy(q);
}

void test_queue_dequeue_multiple(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    enqueue(q, &data);
    enqueue(q, &data2);
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);
    TEST_ASSERT_TRUE(dequeue(q) == &data3);
    queue_destroy(q);
}

// More tests (especially related to shutdown behavior) added as suggested by Flynn Hoare

void test_queue_dequeue_shutdown(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    enqueue(q, &data);
    enqueue(q, &data2);
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);
    queue_shutdown(q);
    TEST_ASSERT_TRUE(dequeue(q) == &data3);
    TEST_ASSERT_TRUE(is_shutdown(q));
    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

void test_enqueue_after_shutdown(void) {
    queue_t q = queue_init(2);
    TEST_ASSERT_NOT_NULL(q);

    int data = 10;
    queue_shutdown(q);
    enqueue(q, &data);

    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

void test_dequeue_empty_after_shutdown(void) {
    queue_t q = queue_init(2);
    TEST_ASSERT_NOT_NULL(q);

    queue_shutdown(q);
    void *item = dequeue(q);

    TEST_ASSERT_NULL(item);
    queue_destroy(q);
}

void test_is_empty_and_is_shutdown_flags(void) {
    queue_t q = queue_init(2);
    TEST_ASSERT_TRUE(is_empty(q));
    TEST_ASSERT_FALSE(is_shutdown(q));

    int data = 100;
    enqueue(q, &data);
    TEST_ASSERT_FALSE(is_empty(q));

    dequeue(q);
    TEST_ASSERT_TRUE(is_empty(q));

    queue_shutdown(q);
    TEST_ASSERT_TRUE(is_shutdown(q));
    queue_destroy(q);
}

void test_queue_wraparound_behavior(void) {
    queue_t q = queue_init(3);
    TEST_ASSERT_NOT_NULL(q);

    int data1 = 1, data2 = 2, data3 = 3, data4 = 4;

    enqueue(q, &data1);
    enqueue(q, &data2);
    enqueue(q, &data3);

    TEST_ASSERT_EQUAL_PTR(&data1, dequeue(q));

    enqueue(q, &data4);

    TEST_ASSERT_EQUAL_PTR(&data2, dequeue(q));
    TEST_ASSERT_EQUAL_PTR(&data3, dequeue(q));
    TEST_ASSERT_EQUAL_PTR(&data4, dequeue(q));

    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_destroy);
    RUN_TEST(test_queue_dequeue);
    RUN_TEST(test_queue_dequeue_multiple);
    RUN_TEST(test_queue_dequeue_shutdown);
    RUN_TEST(test_enqueue_after_shutdown);
    RUN_TEST(test_dequeue_empty_after_shutdown);
    RUN_TEST(test_is_empty_and_is_shutdown_flags);
    RUN_TEST(test_queue_wraparound_behavior);
    return UNITY_END();
}
