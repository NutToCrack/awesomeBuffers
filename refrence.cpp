#include<iostream>
#include<tuple>
#include<memory>
#include<utility>
#include<thread>
#include<atomic>
#include<pthread.h>
#include<random>
#include<queue>
#include<mutex>
#include<string>
#include<semaphore.h>


#define ASSERT_PTHREAD(_expression) (_expression) ? report_pthread_error(#_expression) : void(0)

[[noreturn]] void report_pthread_error(char const* _expression) {
    std::cout << "Error in line " << 0 << " in call to: " << _expression << std::endl;
    std::abort();
}


class Thread {
private:
    template<typename _Callable, typename... _Arguments>
    struct Thread_argument {
	_Callable callable;
	std::tuple<_Arguments...> arguments;
    };

public:
    Thread(auto&& _callable, auto&&... _arguments) {
	using Thread_argument_type = Thread_argument<typename std::decay<decltype(_callable)>::type, typename std::decay<decltype(_arguments)>::type...>;


	auto thread_argument = new Thread_argument_type{std::forward<decltype(_callable)>(_callable), {std::forward<decltype(_arguments)>(_arguments)...}};
	ASSERT_PTHREAD(pthread_create(&thread, nullptr, &thread_function<Thread_argument_type>, static_cast<void*>(thread_argument)));
    }

    ~Thread() {
	ASSERT_PTHREAD(pthread_join(thread, nullptr));
    }

    Thread(Thread const&) = delete;
    Thread& operator=(Thread const&) = delete;

private:

    template<typename _Thread_argument_type>
    static void* thread_function(void* _arg) {
	auto thread_argument = std::unique_ptr<_Thread_argument_type>(static_cast<_Thread_argument_type*>(_arg));
	std::apply(thread_argument->callable, thread_argument->arguments);
	return nullptr;
    }

    pthread_t thread;

};

class Mutex {
public:
    Mutex() {
	ASSERT_PTHREAD(pthread_mutex_init(&mutex, nullptr));
    }

    ~Mutex() {
	ASSERT_PTHREAD(pthread_mutex_destroy(&mutex));
    }

    void lock() {
	ASSERT_PTHREAD(pthread_mutex_lock(&mutex));
    }

    void unlock() {
	ASSERT_PTHREAD(pthread_mutex_unlock(&mutex));
    }

    Mutex(Mutex const&) = delete;
    Mutex& operator=(Mutex const&) = delete;

private:
    pthread_mutex_t mutex;

};


class Semaphore  {
public:
    Semaphore(int _value) {
	ASSERT_PTHREAD(sem_init(&semaphore, 0, _value));
    }

    ~Semaphore() {
	ASSERT_PTHREAD(sem_destroy(&semaphore));
    }

    void post() {
	ASSERT_PTHREAD(sem_post(&semaphore));
    }

    void wait() {
	ASSERT_PTHREAD(sem_wait(&semaphore));
    }

    Semaphore(Semaphore const&) = delete;
    Semaphore& operator=(Semaphore const&) = delete;

private:
    sem_t semaphore;

};

class FIFO {
  std::size_t size_max;
  std::queue<int> q;
  Mutex q_mx;
  Semaphore empty_count;
  Semaphore fill_count;

public:
  FIFO(std::size_t size):
      size_max(size), empty_count(size), fill_count(0)
  {}
  ~FIFO() = default;

  void put(int value)
  {
      empty_count.wait();
      {
        std::scoped_lock lock(q_mx);
        q.push(value);
      }
      fill_count.post();
  }

  int get()
  {
      int value;
      fill_count.wait();
      {
        std::scoped_lock lock(q_mx);
        value = q.front();
        q.pop();
      }
      empty_count.post();
      return value;
  }

};

int main() {
  FIFO queues[3] = {10, 10, 10};

    static auto sleep_ms = [](unsigned _ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(_ms));
    };

    std::cout << "Starting" << std::endl;

    {

	Thread producer1(
	    [&]() {
            std::mt19937 mt_rand(time(0));
            std::uniform_int_distribution<int> distribution(1, 100);
		for(;;) {
        {
          int number = distribution(mt_rand) & ~1u;
          queues[0].put(number);
          std::string mess = "Producent 1 wstawil do kolejki 1 " + std::to_string(number) + "\n";
          std::cout<<mess<<std::flush;
        }
        sleep_ms(300);
        {
          int number = distribution(mt_rand) & ~1u;
          queues[1].put(number);
          std::string mess = "Producent 1 Wstawil do kolejki 2 " + std::to_string(number) + "\n";
          std::cout<<mess<<std::flush;
        }
		    sleep_ms(500);
		}
	    }
	);

	Thread producer2(
	    [&queues]() {
            std::mt19937 mt_rand(time(0));
            std::uniform_int_distribution<int> distribution(1, 100);
		for(;;) {
        {
          int number = distribution(mt_rand) | 1u;
          queues[1].put(number);
          std::string mess = "Producent 2 wstawil do kolejki 2 " + std::to_string(number) + "\n";
          std::cout<<mess<<std::flush;
        }
        sleep_ms(200);
        {
          int number = distribution(mt_rand) | 1u;
          queues[2].put(number);
          std::string mess = "Producent 2 wstawil do kolejki 3 " + std::to_string(number) + "\n";
          std::cout<<mess<<std::flush;
        }
		    sleep_ms(300);
		}
	    }
	);

	Thread consumer1(
	    [&]() {
		for(;;) {
        int value = queues[0].get();
        std::string mess ="Zdjelismy z kolejki 1 " + std::to_string(value) + "\n";
        std::cout<<mess<<std::flush;
		    sleep_ms(100);
		}
	    }
	);

	Thread consumer2(
	    [&]() {
		for(;;) {
        int value = queues[1].get();
        std::string mess ="Zdjelismy z kolejki 2 " + std::to_string(value) + "\n";
        std::cout<<mess<<std::flush;
		    sleep_ms(50);
		}
	    }
	);

	Thread consumer3(
	    [&]() {
		for(;;) {
        int value = queues[2].get();
        std::string mess ="Zdjelismy z kolejki 3 " + std::to_string(value) + "\n";
        std::cout<<mess<<std::flush;
		    sleep_ms(150);
		}
	    }
	);
    }

    std::cout << "Done" << std::endl;

}

