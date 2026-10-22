#include "game/worldinfo.h"

void WorldInfo::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_mode", "in_mode"), &WorldInfo::set_mode);
	ClassDB::bind_method(D_METHOD("mode_type"), &WorldInfo::get_selected_mode);
}

void WorldInfo::_get_property_list(List<PropertyInfo>* p_list) const
{
    PackedStringArray matching_classes;
    PackedStringArray all_classes = ClassDB::get_class_list();

    String target_base_type = "Mode";

    for (int i = 0; i < all_classes.size(); i++)
    {
        String class_name = all_classes[i];

        if (ClassDB::is_parent_class(class_name, target_base_type) || class_name == target_base_type)
        {
            matching_classes.append(class_name);
        }
    }

    String hint_string = String(",").join(matching_classes);

    p_list->push_back(PropertyInfo(
        Variant::STRING,
        "mode_type",
        PROPERTY_HINT_ENUM,
        hint_string,
        PROPERTY_USAGE_DEFAULT
    ));
}

bool WorldInfo::_set(const StringName& p_name, const Variant& p_value)
{
    if (p_name == String("mode_type"))
    {
        mode_type = p_value;
        notify_property_list_changed(); // Forces inspector update
        return true;
    }

    return false;
}

bool WorldInfo::_get(const StringName& p_name, Variant& r_ret) const
{
    if (p_name == String("mode_type"))
    {
        r_ret = mode_type;
        return true;
    }

    return false;
}