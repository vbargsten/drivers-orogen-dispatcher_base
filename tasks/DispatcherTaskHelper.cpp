/* Generated from orogen/lib/orogen/templates/DispatcherTaskHelpers/DispatcherTaskHelper.cpp */

#include "DispatcherTaskHelper.hpp"
#include <base-logging/Logging.hpp>

using namespace dispatcher_base;
using namespace std;

template <typename T>
DispatcherTaskHelper<T>::DispatcherTaskHelper(RTT::TaskContext* task)
{
    this->task = task;
}

template <typename T>
DispatcherTaskHelper<T>::~DispatcherTaskHelper()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See DispatcherTaskHelper.hpp for more detailed
// documentation about them.
template <typename T>
bool DispatcherTaskHelper<T>::configureHook(std::vector< ::dispatcher_base::OutputConfiguration >& output_config, std::vector< ::dispatcher_base::SingleDispatchConfiguration > dispatch_config)
{

    mDispatcher = Dispatcher<T>();
    clearPorts(); // make sure all created ports are removed first, in case we aborted one configureHook already

    { // Create the outputs
//         vector<OutputConfiguration> output_config(_outputs.get());
        for (size_t i = 0; i < output_config.size(); ++i)
        {
            OutputConfiguration const& conf(output_config[i]);
            if (task->getPort(conf.name))
            {
                LOG_ERROR("output port %s is listed more than once in the outputs configuration property", conf.name.c_str());
                clearPorts();
                return false;
            }
            OutputPort* port = new OutputPort(conf.name);
            mOutputPorts.push_back(port);
            task->addPort(*port);

            if (conf.jointNames.empty())
            {
                LOG_ERROR("the jointNames field for the output port %s is empty", conf.name.c_str());
                return false;
            }
            mDispatcher.addOutput(conf.name, conf.jointNames);
        }
    }

    { // Now create the inputs *and* the dispatches at the same time
//         vector<SingleDispatchConfiguration> dispatch_config(_dispatches.get());
        for (size_t i = 0; i < dispatch_config.size(); ++i)
        {
            SingleDispatchConfiguration const& conf(dispatch_config[i]);
            if (!task->getPort(conf.input))
            {
                InputPort* port = new InputPort(conf.input);
                mInputPorts.push_back(port);
                task->addEventPort(*port);
                mDispatcher.addInput(conf.input);
            }
            else if (dynamic_cast<InputPort*>(task->getPort(conf.input)) == 0)
            {
                LOG_ERROR("dispatch input %s is an output", conf.input.c_str());
                return false;
            }

            if (!task->getPort(conf.output))
            {
                LOG_ERROR("output port %s is used in the dispatch configuration, but does not have a corresponding output configuration", conf.output.c_str());
                return false;
            }

            ElementSelection<T> in_sel;
            in_sel.byName = conf.input_selection_by_name;
            in_sel.byIndex = conf.input_selection_by_index;
            ElementSelection<T> out_sel;
            out_sel.byName = conf.output_selection_by_name;
            out_sel.byIndex = conf.output_selection_by_index;
            mDispatcher.addDispatch(conf.input, in_sel, conf.output, out_sel, conf.defer_output);
	    
	    for(size_t j=0; j<conf.input_selection_by_name.size();j++)
	    {
		jointToStreamMap.insert(std::make_pair(conf.input_selection_by_name[j],conf.input));
	    }
        }
    }
    

    
    return true;
}

template <typename T>
bool DispatcherTaskHelper<T>::startHook()
{
//     if (! TaskBase::startHook())
//         return false;

    mDispatcher.reset();

    /*{ //Create default Configurations for joints
        vector<DefaultJointConfiguration> config();//_defaultConfiguration.get());
        for (size_t i = 0; i < config.size(); ++i)
        {
            DefaultJointConfiguration const& conf(config[i]);

            for (size_t j = 0; j < mOutputPorts.size(); ++j)
            {
                auto id = mDispatcher.getOutputByName(mOutputPorts[j]->getName());
                
                Output<T> &out(mDispatcher.getOutput(id)); 
                
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

template <typename T>
void DispatcherTaskHelper<T>::updateHook()
{
//     TaskBase::updateHook();

    for (size_t i = 0; i < mInputPorts.size(); ++i)
    {
        while (mInputPorts[i]->read(mJoint, false) == RTT::NewData)
        {
            try
            {
                mDispatcher.write(mInputPorts[i]->getName(), mJoint);//, mJoint.time
            } catch (typename base::NamedVector<T>::InvalidName iv)
            {
                LOG_ERROR("Name not found while reading data from input port %s ", mInputPorts[i]->getName().c_str());
                LOG_ERROR("Names in input are :");
                for(std::vector<std::string>::const_iterator it = mJoint.names.begin(); it != mJoint.names.end(); it++)
                {
                    LOG_ERROR("  %s", it->c_str());
                }
                throw iv;
            }
        }
    }

    for (size_t i = 0; i < mOutputPorts.size(); ++i)
    {
        if (mDispatcher.read(mOutputPorts[i]->getName(), mJoint))//, mJoint.time
        {
            mOutputPorts[i]->write(mJoint);
        }
    }
}

template <typename T>
void DispatcherTaskHelper<T>::errorHook()
{
}
template <typename T>
void DispatcherTaskHelper<T>::stopHook()
{
}
template <typename T>
void DispatcherTaskHelper<T>::cleanupHook()
{
    clearPorts();
}

template <typename T>
void DispatcherTaskHelper<T>::clearPorts()
{
    for (size_t i = 0; i < mInputPorts.size(); ++i)
    {
        task->ports()->removePort(mInputPorts[i]->getName());
        delete mInputPorts[i];
    }
    mInputPorts.clear();

    for (size_t i = 0; i < mOutputPorts.size(); ++i)
    {
        task->ports()->removePort(mOutputPorts[i]->getName());
        delete mOutputPorts[i];
    }
    mOutputPorts.clear();
}

