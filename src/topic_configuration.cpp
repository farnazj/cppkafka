#include "topic_configuration.h"
#include <librdkafka/rdkafka.h>
#include "exceptions.h"

using std::string;

namespace cppkafka {

TopicConfiguration::TopicConfiguration() 
: handle_(make_handle(rd_kafka_topic_conf_new())) {

}

TopicConfiguration::TopicConfiguration(rd_kafka_topic_conf_t* ptr) 
: handle_(make_handle(ptr)) {

}

TopicConfiguration::TopicConfiguration(const TopicConfiguration& rhs) 
: handle_(make_handle(rd_kafka_topic_conf_dup(rhs.handle_.get()))) {

}

TopicConfiguration& TopicConfiguration::operator=(const TopicConfiguration& rhs) {
    handle_.reset(rd_kafka_topic_conf_dup(rhs.handle_.get()));
}

void TopicConfiguration::set(const string& name, const string& value) {
    char error_buffer[512];
    rd_kafka_conf_res_t result;
    result = rd_kafka_topic_conf_set(handle_.get(), name.data(), value.data(), error_buffer,
                                     sizeof(error_buffer));
    if (result != RD_KAFKA_CONF_OK) {
        throw KafkaConfigException(name, error_buffer);
    }
}

TopicConfiguration::HandlePtr TopicConfiguration::make_handle(rd_kafka_topic_conf_t* ptr) {
    return HandlePtr(ptr, &rd_kafka_topic_conf_destroy);
}     

} // cppkafka
