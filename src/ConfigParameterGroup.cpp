#include "WiFiConfig.h"

void ConfigParameterGroup::toJson(JsonObject *json)
{

    std::list<ConfigParameterInterface *>::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it)
    {
        (*it)->toJson(&json);
    }
}

void ConfigParameterGroup::toJsonSchema(JsonObject *json)
{
    json->getOrAddMember(name).set((const char*) name);
    if (this->metadata != NULL)
    {
        // json->set("label", this->metadata->label());
        json->getOrAddMember(label).set((const char*) this->metadata->label());

        if (this->metadata->description() != NULL)
        {
            // json->set("description", this->metadata->description());
            json->getOrAddMember(description).set((const char*) this->metadata->description());

        }
    }

    JsonArray params = json.createNestedArray("params");
    std::list<ConfigParameterInterface *>::iterator it;
    for (it = parameters.begin(); it != parameters.end(); ++it)
    {
        JsonObject obj = params.createNestedObject();
        (*it)->toJsonSchema(&obj);
    }
}

void ConfigParameterGroup::fromJson(JsonObject *json)
{
    if (json->containsKey(name) && json->getMember(name).is<JsonObject>())
    {
        // JsonVariant var = json->get<JsonVariant>(name);
        this->update(json->getMember(name).as<JsonVariant>());

        JsonObject  group = var.asObject();

        std::list<ConfigParameterInterface *>::iterator it;
        for (it = parameters.begin(); it != parameters.end(); ++it)
        {
            (*it)->fromJson(&group);
        }
    }
}
