#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
    // Worker threads koji stalno čekaju i izvršavaju zadatke
    std::vector<std::thread> workers;

    // Thread-safe queue zadataka (FIFO struktura)
    // Svaki task je funkcija bez parametara i povratne vrijednosti
    std::queue<std::function<void()>> tasks;

    // Mutex osigurava da samo jedan thread u isto vrijeme može pristupiti queue-u
    std::mutex mtx;

    // Condition variable omogućuje worker threadovima da "spavaju"
    // dok nema zadataka (izbjegava busy-waiting / CPU spinning)
    std::condition_variable cv;

    // Signal koji govori threadovima da se pool gasi
    bool stop;

public:

    ThreadPool(size_t numThreads)
        : stop(false)
    {
        // Kreiranje fiksnog broja worker threadova
        for (size_t i = 0; i < numThreads; i++)
        {
            workers.emplace_back([this]()
            {
                // Svaki worker thread radi beskonačnu petlju dok se ne zaustavi
                while (true)
                {
                    std::function<void()> task;

                    {
                        // Kritična sekcija: pristup shared queue-u
                        std::unique_lock<std::mutex> lock(mtx);

                        // Thread spava dok:
                        // - nema taskova
                        // - i pool nije u stop stanju
                        cv.wait(lock, [this]()
                        {
                            return stop || !tasks.empty();
                        });

                        // Ako je pool zaustavljen i nema taskova, thread izlazi
                        if (stop && tasks.empty())
                        {
                            return;
                        }

                        // Uzimanje sljedećeg zadatka iz queue-a (FIFO)
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    // Izvršavanje taska izvan lock-a
                    // (bitno da ne blokira ostale threadove)
                    task();
                }
            });
        }
    }

    // Dodavanje novog zadatka u queue
    void enqueue(std::function<void()> task)
    {
        {
            // Lock štiti queue od race conditiona
            std::lock_guard<std::mutex> lock(mtx);

            // Dodavanje taska u red čekanja
            tasks.push(std::move(task));
        }

        // Budimo jedan thread koji čeka na condition variable
        cv.notify_one();
    }

    ~ThreadPool()
    {
        {
            // Signal da se pool gasi
            // mora biti zaštićen mutexom zbog thread safety
            std::lock_guard<std::mutex> lock(mtx);

            stop = true;
        }

        // Budi sve threadove da mogu provjeriti stop uvjet
        cv.notify_all();

        // Čekamo da svi worker threadovi završe (clean shutdown)
        for (auto& worker : workers)
        {
            worker.join();
        }
    }
};

int main()
{
    // Kreiranje thread poola s 4 worker thread-a
    ThreadPool pool(4);

    // Dodavanje 10 taskova u queue
    for (int i = 0; i < 10; i++)
    {
        pool.enqueue([i]()
        {
            std::cout << "Task " << i << " executed\n";
        });
    }

    // Destructor se automatski poziva ovdje i čisti threadove
    return 0;
}