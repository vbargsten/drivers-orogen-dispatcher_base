#ifndef joint_dispatcher_TYPES_HPP
#define joint_dispatcher_TYPES_HPP

#include <vector>
#include <string>

namespace joint_dispatcher {
    struct DispatchConfiguration
    {
        /** Name of the input port */
        std::string input;
        /** Selection of joints on the input port, by index. If you want to
         * select by name use input_selection_by_name instead
         */
        std::vector<unsigned int> input_selection_by_index;
        /** Selection of joints on the input port, by name. If you want to
         * select by index, use input_selection_by_index instead
         */
        std::vector<std::string> input_selection_by_name;

        /** Name of the output port */
        std::string output;
        /** Selection of joints on the output port, by index. If you want to
         * select by name use output_selection_by_name instead
         */
        std::vector<unsigned int> output_selection_by_index;
        /** Selection of joints on the output port, by name. If you want to
         * select by index, use output_selection_by_index instead
         */
        std::vector<std::string> output_selection_by_name;
    };
}

#endif

