#include <iostream>
#include <thread>
#include <functional>
#include <queue>

enum Command : int
{
    None = 0,
    Cleaning = 1,
    FullSpeedAhead = 2,
    StopEngine = 3,
    Exit = 100
};

template<typename Iteratable>
class IteratableThreadGuard
{
public:
    IteratableThreadGuard(
        std::function<void(Iteratable&)> iterate,
        Iteratable & obj
    )
    {
        m_thread = std::thread(iterate, std::ref(obj));
    }
    
    void join()
    {
        if (m_thread.joinable())
            m_thread.join();
    }
    
    ~IteratableThreadGuard()
    {
        join();
    }
    
private:
    std::thread m_thread;
};

class CleaningCrew
{
public:
    CleaningCrew()
     : m_threadGuard(&CleaningCrew::iterate, *this)
    {}

    void clean()
    {
        m_tasks.push(Cleaning);
    }
    
    void dropTasks()
    {
        auto emptyTasks = std::queue<Command>();
        std::swap(m_tasks, emptyTasks);
        m_tasks.push(Exit);
        m_threadGuard.join();
    }
    
protected:
    void iterate()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            if (m_tasks.empty())
                continue;
            
            switch (m_tasks.front())
            {
                case Exit:
                {
                    return;
                }
                break;
                case Cleaning:
                {
                    // TODO: simulateCleaining
                }
                break;
                default:
                {
                    // Wait for valid command
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
            }
        }
    }

private:
    IteratableThreadGuard<CleaningCrew> m_threadGuard;
    std::queue<Command> m_tasks;
};

class EngineCrew
{
public:
    EngineCrew()
     : m_threadGuard(&EngineCrew::iterate, *this)
    {}

    void startEngine()
    {
        m_tasks.push(FullSpeedAhead);
    }

    void stopEngine()
    {
        m_tasks.push(StopEngine);
    }
    
    void dropTasks()
    {
        auto emptyTasks = std::queue<Command>();
        std::swap(m_tasks, emptyTasks);
        m_tasks.push(Exit);
        m_threadGuard.join();
    }
    
protected:
    void iterate()
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            if (m_tasks.empty())
                continue;
            
            switch (m_tasks.front())
            {
                case Exit:
                {
                    return;
                }
                break;
                case FullSpeedAhead:
                {
                    // TODO: simulateEngineStarting
                }
                break;
                case StopEngine:
                {
                    // TODO: simulateEngineStopping
                }
                break;
                default:
                {
                    // Wait for valid command
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }
            }
        }
    }

private:
    IteratableThreadGuard<EngineCrew> m_threadGuard;
    std::queue<Command> m_tasks;
};

class Captain
{
public:
    void chooseCommand()
    {
        int input;
        
        while (runCommand(input)) ;
    }

private:
    CleaningCrew m_cleaningCrew;
    EngineCrew m_engineCrew;

    bool runCommand(int input)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        std::cout << "\nYou are a captain, choose your command to the crew\n"
                     "(1 - clean the boat, 2 - full speed ahead,\n"
                     " 3 - stop the engine, 100 - exit the program)\n";
        std::cout << "Enter command > ";
        std::cin >> input;
        
        switch (input)
        {
            case Cleaning:
            {
                m_cleaningCrew.clean();
            }
            break;
            case FullSpeedAhead:
            {
                m_engineCrew.startEngine();
            }
            break;
            case StopEngine:
            {
                m_engineCrew.stopEngine();
            }
            break;
            case Exit:
            {
                m_cleaningCrew.dropTasks();
                m_engineCrew.dropTasks();
                return false;
            }
            break;
            default:
            {
                std::cout << "Command '" << input << "' is invalid, try again\n";
            }
        }
        
        return true;
    }
};

int main()
{
    Captain cap;
    cap.chooseCommand();

    return 0;
}