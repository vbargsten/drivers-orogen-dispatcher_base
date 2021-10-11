/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */
#include "DispatcherTaskHelper.cpp"
#include "Task.hpp"
#include <base-logging/Logging.hpp>
#include <dispatcher_base/Dispatcher_impl.hpp>

using namespace dispatcher_base;
using namespace std;


Task::Task(std::string const& name)
    : TaskBase(name), dispatcherHelper(this)
{
}


Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine), dispatcherHelper(this)
{
}


Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.
// /*
bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;
    
    dispatcherHelper.configureHook( _outputs.value(), _dispatches.value() );

    /*mDispatcher = Dispatcher();
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

            ElementSelection in_sel;
            in_sel.byName = conf.input_selection_by_name;
            in_sel.byIndex = conf.input_selection_by_index;
            ElementSelection out_sel;
            out_sel.byName = conf.output_selection_by_name;
            out_sel.byIndex = conf.output_selection_by_index;
            mDispatcher.addDispatch(conf.input, in_sel, conf.output, out_sel, conf.defer_output);
	    
	    for(size_t j=0; j<conf.input_selection_by_name.size();j++)
	    {
		jointToStreamMap.insert(std::make_pair(conf.input_selection_by_name[j],conf.input));
	    }
        }
    }*/
    

    
    return true;
}


bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;

    /*mDispatcher.reset();    { //Create default Configurations for joints
        vector<DefaultJointConfiguration> config(_defaultConfiguration.get());
        for (size_t i = 0; i < config.size(); ++i)
        {
            DefaultJointConfiguration const& conf(config[i]);

            for (size_t j = 0; j < mOutputPorts.size(); ++j)
            {
                Dispatcher::ChannelID id = mDispatcher.getOutputByName(mOutputPorts[j]->getName());
                
                Output &out(mDispatcher.getOutput(id)); 
                
                std::vector<std::string> names = out.getNames();
                bool found = false;

                for(size_t k = 0; k < names.size(); ++k)
                {
                    if(names[k] == conf.jointName)
                    {
                        found = true;
                        break;
                    }
                }
                
                if(!found)
                    continue;
                
                names.clear();
                names.push_back(conf.jointName);
                    
                std::vector<size_t> idx = out.mapNamesToIndex(names);

                out.updateJoint(idx[0], base::Time::now(), T::Position(conf.position));
            }
        }
    }*/
    return true;
}


void Task::updateHook()
{
    TaskBase::updateHook();    
    dispatcherHelper.updateHook();
}


void Task::errorHook()
{
    TaskBase::errorHook();
    dispatcherHelper.errorHook();
}

void Task::stopHook()
{
    TaskBase::stopHook();
    dispatcherHelper.stopHook();
}

void Task::cleanupHook()
{
    TaskBase::cleanupHook();
    dispatcherHelper.cleanupHook();
}

