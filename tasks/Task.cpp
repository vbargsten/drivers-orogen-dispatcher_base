/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <base/Logging.hpp>

using namespace joint_dispatcher;
using namespace std;

Task::Task(std::string const& name)
    : TaskBase(name)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;

    mDispatcher = Dispatcher();
    clearPorts(); // make sure all created ports are removed first, in case we aborted one configureHook already

    { // Create the outputs
        vector<OutputConfiguration> config(_outputs.get());
        for (size_t i = 0; i < config.size(); ++i)
        {
            OutputConfiguration const& conf(config[i]);
            if (getPort(conf.name))
            {
                LOG_ERROR("output port %s is listed more than once in the outputs configuration property", conf.name.c_str());
                clearPorts();
                return false;
            }
            OutputPort* port = new OutputPort(conf.name);
            mOutputPorts.push_back(port);
            addPort(*port);

            if (conf.jointNames.empty())
            {
                LOG_ERROR("the jointNames field for the output port %s is empty", conf.name.c_str());
                return false;
            }
            mDispatcher.addOutput(conf.name, conf.jointNames);
        }
    }

    { // Now create the inputs *and* the dispatches at the same time
        vector<SingleDispatchConfiguration> config(_dispatches.get());
        for (size_t i = 0; i < config.size(); ++i)
        {
            SingleDispatchConfiguration const& conf(config[i]);
            if (!getPort(conf.input))
            {
                InputPort* port = new InputPort(conf.input);
                mInputPorts.push_back(port);
                addEventPort(*port);
                mDispatcher.addInput(conf.input);
            }
            else if (dynamic_cast<InputPort*>(getPort(conf.input)) == 0)
            {
                LOG_ERROR("dispatch input %s is an output", conf.input.c_str());
                return false;
            }

            if (!getPort(conf.output))
            {
                LOG_ERROR("output port %s is used in the dispatch configuration, but does not have a corresponding output configuration", conf.output.c_str());
                return false;
            }

            JointSelection in_sel;
            in_sel.byName = conf.input_selection_by_name;
            in_sel.byIndex = conf.input_selection_by_index;
            JointSelection out_sel;
            out_sel.byName = conf.output_selection_by_name;
            out_sel.byIndex = conf.output_selection_by_index;
            mDispatcher.addDispatch(conf.input, in_sel, conf.output, out_sel);
        }
    }
    
    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;

    mDispatcher.reset();
    return true;
}
void Task::updateHook()
{
    TaskBase::updateHook();

    for (size_t i = 0; i < mInputPorts.size(); ++i)
    {
        while (mInputPorts[i]->read(mJoint, false) == RTT::NewData)
            mDispatcher.write(mInputPorts[i]->getName(), mJoint);
    }

    for (size_t i = 0; i < mOutputPorts.size(); ++i)
    {
        if (mDispatcher.read(mOutputPorts[i]->getName(), mJoint))
            mOutputPorts[i]->write(mJoint);
    }
}
void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    clearPorts();
    TaskBase::cleanupHook();
}

void Task::clearPorts()
{
    for (size_t i = 0; i < mInputPorts.size(); ++i)
    {
        ports()->removePort(mInputPorts[i]->getName());
        delete mInputPorts[i];
    }
    mInputPorts.clear();

    for (size_t i = 0; i < mOutputPorts.size(); ++i)
    {
        ports()->removePort(mOutputPorts[i]->getName());
        delete mOutputPorts[i];
    }
    mOutputPorts.clear();
}
