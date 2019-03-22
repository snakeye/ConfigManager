#include "ConfigManager.h"

void ConfigParameterGroup::toJson(JsonObject *json)
{
    JsonObject &obj = json->createNestedObject(this->name);

    std::list<BaseParameter *>::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it)
    {
        (*it)->toJson(&obj);
    }
}

void ConfigParameterGroup::toJsonSchema(JsonObject *json)
{
    json->set("name", name);
    if (this->metadata != NULL)
    {
        json->set("label", this->metadata->label());

        if (this->metadata->description() != NULL)
        {
            json->set("description", this->metadata->description());
        }
    }

    JsonArray &params = json->createNestedArray("params");
    std::list<BaseParameter *>::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it)
    {
        JsonObject &obj = params.createNestedObject();
        (*it)->toJsonSchema(&obj);
    }
}

void ConfigParameterGroup::fromJson(JsonObject *json)
{
    if (json->containsKey(name) && json->is<JsonObject>(name))
    {
        JsonVariant var = json->get<JsonVariant>(name);

        JsonObject &group = var.asObject();

        std::list<BaseParameter *>::iterator it;
        for (it = parameters.begin(); it != parameters.end(); ++it)
        {
            (*it)->fromJson(&group);
        }
    }
}
