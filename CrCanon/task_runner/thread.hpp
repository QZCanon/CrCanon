#ifndef THREAD_H
#define THREAD_H
#include <thread>

class Thread
{
public:
    virtual ~Thread()
    {
		if(mIsStart && !mIsDetach)
        	detach();
    }
    bool start()
    {
        if (mIsStart) {
            return true;
        }
        mThreadId = std::thread(&Thread::threadRun, this);
        mIsStart = true;
		isRun = true;
        return true;
    }

	void stop()
	{
		isRun = false;
	}

	bool loop() const
	{
		return isRun;
	}

    bool detach()
    {
        if(!mIsStart) {
            return false;
        }

        if(mIsDetach)
            return true;

        mThreadId.detach();

        mIsDetach = true;

        return true;
    }
    bool join()
    {
        if(!mIsStart || mIsDetach)
            return false;

        mThreadId.join();

        return true;
    }

protected:
    Thread() :
        mIsStart(false),
        mIsDetach(false),
		isRun(false)
    {}

    virtual void run() = 0;
private:

    static void *threadRun(void *arg)
    {
        auto* thread = (Thread*)arg;
        thread->run();
        return nullptr;
    }
private:
    bool mIsStart;
    bool mIsDetach;
	bool isRun = false;
    std::thread mThreadId;

};




#endif