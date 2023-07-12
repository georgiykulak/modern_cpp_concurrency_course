#include <iostream>
#include <thread>

class Cleaner
{
public:
    void clean()
    {
        if (m_cleainingInProgress)
        {
            std::cout << "\nThe cleaner is busy with current task ("
                      << m_currentCleanTaskID << ")\n";
            return;
        }
        
        std::thread cleanerTask(&Cleaner::cleanBoat, this);
        cleanerTask.detach();
    }

private:
    bool m_cleainingInProgress = false;
    std::thread::id m_currentCleanTaskID;
    
    void simulateCleaining()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(15000));
    }
    
    void cleanBoat()
    {
        m_cleainingInProgress = true;
        m_currentCleanTaskID = std::this_thread::get_id();
        
        std::cout <<  "\n!! The cleaner started cleaning (" << m_currentCleanTaskID << ")\n";
        simulateCleaining();
        std::cout << "\n!! The cleaner finished cleaning (" << m_currentCleanTaskID << ")\n";
        
        m_cleainingInProgress = false;
    }
};

class EngineCrew
{
public:
    enum Status
    {
        Stopped,
        Started
    };

    ~EngineCrew()
    {
        if (startEngineTask.joinable())
            startEngineTask.join();
            
        if (stopEngineTask.joinable())
            stopEngineTask.join();
    }

    void startEngine()
    {
        if (m_engineStartingIsInProgress)
        {
            std::cout << "\nThe engine is currently starting by the crew ("
                      << m_currentEngineTaskID << ")\n";
            return;
        }
        
        if (m_status == Started)
        {
            std::cout << "\nThe engine is already started\n";
            return;
        }
        
        if (stopEngineTask.joinable())
            stopEngineTask.join();

        startEngineTask = std::thread(&EngineCrew::engineStart, this);
    }
    
    void stopEngine()
    {
        if (m_engineStoppingIsInProgress)
        {
            std::cout << "\nThe engine is currently stopping by the crew ("
                      << m_currentEngineTaskID << ")\n";
            return;
        }
        
        if (m_status == Stopped)
        {
            std::cout << "\nThe engine is already stopped\n";
            return;
        }
        
        if (startEngineTask.joinable())
            startEngineTask.join();
        
        stopEngineTask = std::thread(&EngineCrew::engineStop, this);
    }
    
private:
    std::thread startEngineTask;
    std::thread stopEngineTask;
    std::thread::id m_currentEngineTaskID;
    Status m_status = Stopped;
    bool m_engineStartingIsInProgress = false;
    bool m_engineStoppingIsInProgress = false;
    
    void simulateEngineStarting()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(6000));
    }
    
    void simulateEngineStopping()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    
    void engineStart()
    {
        m_engineStartingIsInProgress = true;
        m_currentEngineTaskID = std::this_thread::get_id();
        
        std::cout << "\n! The crew is starting engine (" << m_currentEngineTaskID << ")\n";
        simulateEngineStarting();
        std::cout << "\n! The crew successfully started engine (" << m_currentEngineTaskID << ")\n";
        
        m_engineStartingIsInProgress = false;
        m_status = Started;
    }
    
    void engineStop()
    {
        m_engineStoppingIsInProgress = true;
        m_currentEngineTaskID = std::this_thread::get_id();
        
        std::cout << "\n! The crew is stopping engine (" << m_currentEngineTaskID << ")\n";
        simulateEngineStopping();
        std::cout << "\n! The crew successfully stopped engine (" << m_currentEngineTaskID << ")\n";
        
        m_engineStoppingIsInProgress = false;
        m_status = Stopped;
    }
};

class Captain
{
private:
    enum Commands : int
    {
        Cleaning = 1,
        FullSpeedAhead = 2,
        StopEngine = 3,
        Exit = 100
    };

public:
    void chooseCommand()
    {
        int input;
        
        while (runCommand(input)) ;
    }

private:
    Cleaner cleaner;
    EngineCrew engineCrew;

    bool runCommand(int input)
    {
        std::cout << "\nYou are a captain, choose your command to the crew\n"
                     "(1 - clean the boat, 2 - full speed ahead,\n"
                     " 3 - stop the engine, 100 - exit the program)\n";
        std::cout << "Enter command > ";
        std::cin >> input;
        
        switch (input)
        {
            case Cleaning:
            {
                cleaner.clean();
            }
            break;
            case FullSpeedAhead:
            {
                engineCrew.startEngine();
            }
            break;
            case StopEngine:
            {
                engineCrew.stopEngine();
            }
            break;
            case Exit:
            {
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