#ifndef joint_dispatcher_TYPES_HPP
#define joint_dispatcher_TYPES_HPP

#include <vector>
#include <string>

namespace joint_dispatcher {
    struct OutputConfiguration
    {
        std::string name;

        /** The name of the joints. Resize to a desired size, but leaving the
         * names empty, if you do not want to use joint names
         */
        std::vector<std::string> jointNames;
    };

    struct SingleDispatchConfiguration
    {
        /** Name of the input port */
        std::string input;
        /** Selection of joints on the input port, by index. If you want to
         * select by name use input_selection_by_name instead
         */
        std::vector<int> input_selection_by_index;
        /** Selection of joints on the input port, by name. If you want to
         * select by index, use input_selection_by_index instead
         */
        std::vector<std::string> input_selection_by_name;

        /** Name of the output port */
        std::string output;
        /** Selection of joints on the output port, by index. If you want to
         * select by name use output_selection_by_name instead
         */
        std::vector<int> output_selection_by_index;
        /** Selection of joints on the output port, by name. If you want to
         * select by index, use output_selection_by_index instead
         */
        std::vector<std::string> output_selection_by_name;
    };
    
    struct DefaultJointConfiguration
    {
      std::string jointName;
      
      double position;
    };
}

#endif

