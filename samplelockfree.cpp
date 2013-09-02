





#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <iostream>

using std::cout;
using std::string;
using std::endl;

class Sample_Lockfree
{
    boost::atomic_int producer_count ;
    boost::atomic_int consumer_count ;
    boost::lockfree::queue<string*> queue;
    static const int iterations = 10; //10000000;
    static const int producer_thread_count = 4;
    static const int consumer_thread_count = 4;
    boost::atomic<bool> done;

public:
    
    Sample_Lockfree ()
        : producer_count(0)
        , consumer_count (0)
        , queue(128)
        , done (false)
    {
    }

    void producer();
    void consumer();
    void test_lockfree();
};



void Sample_Lockfree::producer()
{
    for (int i = 0; i != iterations; ++i)
    {
        int value = ++producer_count;
        string* strValue = new string(std::to_string(value));
        while (!queue.push(strValue))
            ;
        cout << "ADDED:" << strValue << endl;
    }
}

void Sample_Lockfree::consumer()
{
    string* value = NULL;
    while (!done) {
        while (queue.pop(value))
        {
            ++consumer_count;
            cout << "POPPED : " << value << endl;
        }
    }

    while (queue.pop(value))
    {
        ++consumer_count;
        cout << "POPPED_____________ : " << value << endl;
    }
}
void Sample_Lockfree::test_lockfree()
{
    using namespace std;
    cout << "boost::lockfree::queue is ";
    if (!queue.is_lock_free())
        cout << "not ";
    cout << "lockfree" << endl;

    boost::thread_group producer_threads, consumer_threads;
        
    for (int i = 0; i != producer_thread_count; ++i)
        producer_threads.create_thread(std::bind (&Sample_Lockfree::producer, this));
        
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(std::bind (&Sample_Lockfree::consumer, this));
          
    producer_threads.join_all();
    done = true;

    consumer_threads.join_all();

    cout << "produced " << producer_count << " objects." << endl;
    cout << "consumed " << consumer_count << " objects." << endl;
}

int main()
{
   
    Sample_Lockfree lockFreeSample;
    lockFreeSample.test_lockfree();
                    

}
