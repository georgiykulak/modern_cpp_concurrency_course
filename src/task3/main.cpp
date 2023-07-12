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
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            
            switch (m_tasks.front())
            {
                case Exit:
                {
                    return;
                }
                break;
                case Cleaning:
                {
                    cleanBoat();
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
    std::thread::id m_currentCleanTaskID;
    
    void simulateCleaining()
    {
        m_currentCleanTaskID = std::this_thread::get_id();
        
        m_tasks.pop();
        
        std::cout <<  "\n!! The cleaning crew started cleaning (" << m_currentCleanTaskID << ")\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n!! The cleaning crew finished cleaning (" << m_currentCleanTaskID << ")\n";
    }
    
    void cleanBoat()
    {
        if (m_tasks.empty() || m_tasks.front() != Cleaning)
            return;
        
        std::thread cleaningCrewTask(&CleaningCrew::simulateCleaining, this);
        cleaningCrewTask.join();
    }
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
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            
            switch (m_tasks.front())
            {
                case Exit:
                {
                    return;
                }
                break;
                case FullSpeedAhead:
                {
                    engineStart();
                }
                break;
                case StopEngine:
                {
                    engineStop();
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
    std::thread::id m_currentEngineTaskID;
    
    void simulateEngineStarting()
    {
        m_currentEngineTaskID = std::this_thread::get_id();
        
        m_tasks.pop();
        
        std::cout << "\n! The crew is starting engine (" << m_currentEngineTaskID << ")\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n! The crew successfully started engine (" << m_currentEngineTaskID << ")\n";
    }
    
    void simulateEngineStopping()
    {
        m_currentEngineTaskID = std::this_thread::get_id();
        
        m_tasks.pop();
        
        std::cout << "\n! The crew is stopping engine (" << m_currentEngineTaskID << ")\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n! The crew successfully stopped engine (" << m_currentEngineTaskID << ")\n";
    }
    
    void engineStart()
    {
        if (m_tasks.empty()
         || m_tasks.front() != FullSpeedAhead
         || m_tasks.front() != StopEngine)
            return;
        
        std::thread engineCrewTask(&EngineCrew::simulateEngineStarting, this);
        engineCrewTask.join();
    }
    
    void engineStop()
    {
        if (m_tasks.empty()
         || m_tasks.front() != FullSpeedAhead
         || m_tasks.front() != StopEngine)
            return;
        
        std::thread engineCrewTask(&EngineCrew::simulateEngineStopping, this);
        engineCrewTask.join();
    }
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