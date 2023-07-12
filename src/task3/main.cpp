#include <iostream>
#include <thread>
#include <functional>

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
    IteratableThreadGuard(std::function<void(Iteratable&)> iterate, Iteratable & obj)
    {
        m_thread = std::thread(iterate, std::ref(obj));
    }
    
    ~IteratableThreadGuard()
    {
        if (m_thread.joinable())
            m_thread.join();
    }
    
private:
    std::thread m_thread;
};

class CleaningCrew
{
public:
    CleaningCrew(const Command& commandHandle)
     : m_commandHandle(commandHandle)
     , m_threadGuard(&CleaningCrew::iterate, *this)
    {}

    void clean()
    {
        
    }
    
protected:
    void iterate()
    {
        
    }

private:
    const Command& m_commandHandle;
    IteratableThreadGuard<CleaningCrew> m_threadGuard;
};

class EngineCrew
{
public:
    EngineCrew(const Command& commandHandle)
     : m_commandHandle(commandHandle)
     , m_threadGuard(&EngineCrew::iterate, *this)
    {}

    void startEngine()
    {
        
    }

    void stopEngine()
    {
        
    }
    
protected:
    void iterate()
    {
        
    }

private:
    const Command& m_commandHandle;
    IteratableThreadGuard<EngineCrew> m_threadGuard;
};

class Captain
{
public:
    
    Captain()
     : m_currentCommand{None}
     , m_cleaningCrew{m_currentCommand}
     , m_engineCrew{m_currentCommand}
    {}

    void chooseCommand()
    {
        int input;
        
        while (runCommand(input)) ;
    }

private:
    Command m_currentCommand;
    CleaningCrew m_cleaningCrew;
    EngineCrew m_engineCrew;

    bool runCommand(int input)
    {
        std::cout << "\nYou are a captain, choose your command to the crew\n"
                     "(1 - clean the boat, 2 - full speed ahead,\n"
                     " 3 - stop the engine, 100 - exit the program)\n";
        std::cout << "Enter command > ";
        std::cin >> input;
        
        m_currentCommand = static_cast<Command>(input);
        
        if (m_currentCommand == Exit)
            return false;
        
        return true;
    }
};

int main()
{
    Captain cap;
    cap.chooseCommand();

    return 0;
}