
#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include <mqtt-sn/format.h>


uint32_t factorial( uint32_t number ) {
    return number <= 1 ? number : factorial(number-1) * number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( factorial( 1) == 1 );
    REQUIRE( factorial( 2) == 2 );
    REQUIRE( factorial( 3) == 6 );
    REQUIRE( factorial(10) == 3'628'800 );
}

TEST_CASE("Advertise", "[format]") {
    mqtt_sn::Advertise advertise;
    advertise.gateway_id = 1;
    advertise.duration = 2;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(advertise, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Advertise>(msg.value()));
}

TEST_CASE("SearchGateway", "[format]") {
    mqtt_sn::SearchGateway advertise;
    advertise.radius = 1;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(advertise, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::SearchGateway>(msg.value()));
}

TEST_CASE("GatewayInfo", "[format]") {
    mqtt_sn::GatewayInfo advertise;
    advertise.gateway_id = 1;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(advertise, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::GatewayInfo>(msg.value()));
}

TEST_CASE("GatewayInfo2", "[format]") {
    mqtt_sn::GatewayInfo advertise;
    advertise.gateway_id = 1;
    advertise.gateway_addr = std::vector<uint8_t> {1, 2, 3};

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(advertise, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::GatewayInfo>(msg.value()));
}

TEST_CASE("Connect", "[format]") {
    mqtt_sn::Connect connect;
    connect.flags.clean_session = true;
    connect.protocol_version = 1;
    connect.duration = 2;
    connect.client_id = "foo";

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(connect, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Connect>(msg.value()));
}

TEST_CASE("ConnectAck", "[format]") {
    mqtt_sn::ConnectAck connect_ack;
    connect_ack.code = mqtt_sn::MessageErrorCode::Accepted;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(connect_ack, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::ConnectAck>(msg.value()));
}

TEST_CASE("WillTopicRequest", "[format]") {
    mqtt_sn::WillTopicRequest will_topic_request;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_topic_request, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillTopicRequest>(msg.value()));
}

TEST_CASE("WillTopicEmpty", "[format]") {
    mqtt_sn::WillTopicEmpty will_topic_empty;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_topic_empty, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillTopicEmpty>(msg.value()));
}

TEST_CASE("WillTopic", "[format]") {
    mqtt_sn::WillTopic will_topic;
    will_topic.topic = "foo";

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_topic, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillTopic>(msg.value()));
}

TEST_CASE("WillMessageRequest", "[format]") {
    mqtt_sn::WillMessageRequest will_message_request;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_message_request, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillMessageRequest>(msg.value()));
}

TEST_CASE("WillMessage", "[format]") {
    mqtt_sn::WillMessage will_message;
    will_message.payload = std::vector<uint8_t> {1, 2, 3};

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_message, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillMessage>(msg.value()));
}

TEST_CASE("WillTopicUpdateEmpty", "[format]") {
    mqtt_sn::WillTopicUpdateEmpty will_topic_update_empty;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_topic_update_empty, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillTopicUpdateEmpty>(msg.value()));
}

TEST_CASE("WillTopicUpdate", "[format]") {
    mqtt_sn::WillTopicUpdate will_topic_update;
    will_topic_update.topic = "foo";

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_topic_update, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillTopicUpdate>(msg.value()));
}

TEST_CASE("WillTopicResponse", "[format]") {
    mqtt_sn::WillTopicResponse will_topic_response;
    will_topic_response.code = mqtt_sn::MessageErrorCode::Accepted;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_topic_response, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillTopicResponse>(msg.value()));
}

TEST_CASE("WillMessageUpdate", "[format]") {
    mqtt_sn::WillMessageUpdate will_message_update;
    will_message_update.payload = std::vector<uint8_t> {1, 2, 3};

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_message_update, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillMessageUpdate>(msg.value()));
}

TEST_CASE("WillMessageResponse", "[format]") {
    mqtt_sn::WillMessageResponse will_message_response;
    will_message_response.code = mqtt_sn::MessageErrorCode::Accepted;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(will_message_response, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::WillMessageResponse>(msg.value()));
}

TEST_CASE("RegisterTopic", "[format]") {
    mqtt_sn::RegisterTopic register_topic;
    register_topic.topic_id = 1;
    register_topic.message_id = 2;
    register_topic.topic = "foo";

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(register_topic, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::RegisterTopic>(msg.value()));
}

TEST_CASE("RegisterTopicAck", "[format]") {
    mqtt_sn::RegisterTopicAck register_topic_ack;
    register_topic_ack.topic_id = 1;
    register_topic_ack.message_id = 2;
    register_topic_ack.code = mqtt_sn::MessageErrorCode::Accepted;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(register_topic_ack, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::RegisterTopicAck>(msg.value()));
}

TEST_CASE("PublishMessage", "[format]") {
    mqtt_sn::PublishMessage publish_message;
    publish_message.topic_id = 1;
    publish_message.message_id = 2;
    publish_message.payload = std::vector<uint8_t> {1, 2, 3};

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(publish_message, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PublishMessage>(msg.value()));
}

TEST_CASE("PublishMessageAck", "[format]") {
    mqtt_sn::PublishMessageAck publish_message_ack;
    publish_message_ack.topic_id = 1;
    publish_message_ack.message_id = 2;
    publish_message_ack.code = mqtt_sn::MessageErrorCode::Accepted;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(publish_message_ack, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PublishMessageAck>(msg.value()));
}

TEST_CASE("PublishMessageComplete", "[format]") {
    mqtt_sn::PublishMessageComplete publish_message_complete;
    publish_message_complete.message_id = 1;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(publish_message_complete, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PublishMessageComplete>(msg.value()));
}

TEST_CASE("PublishMessageReceived", "[format]") {
    mqtt_sn::PublishMessageReceived publish_message_received;
    publish_message_received.message_id = 1;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(publish_message_received, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PublishMessageReceived>(msg.value()));
}

TEST_CASE("PublishMessageRelease", "[format]") {
    mqtt_sn::PublishMessageRelease publish_message_release;
    publish_message_release.message_id = 1;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(publish_message_release, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PublishMessageRelease>(msg.value()));
}

TEST_CASE("Subscribe", "[format]") {
    mqtt_sn::Subscribe subscribe;
    subscribe.flags.topic_id_type = static_cast<uint8_t>(mqtt_sn::TopicIdType::Normal);
    subscribe.message_id = 1;
    subscribe.topic = "foo";

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(subscribe, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Subscribe>(msg.value()));
}

TEST_CASE("Subscribe2", "[format]") {
    mqtt_sn::Subscribe subscribe;
    subscribe.flags.topic_id_type = static_cast<uint8_t>(mqtt_sn::TopicIdType::PreDefined);
    subscribe.message_id = 1;
    subscribe.topic = 2;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(subscribe, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Subscribe>(msg.value()));
}

TEST_CASE("Subscribe3", "[format]") {
    mqtt_sn::Subscribe subscribe;
    subscribe.flags.topic_id_type = static_cast<uint8_t>(mqtt_sn::TopicIdType::Short);
    subscribe.message_id = 1;
    subscribe.topic = 0x7474; // "tt"

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(subscribe, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Subscribe>(msg.value()));
}

TEST_CASE("SubscribeAck", "[format]") {
    mqtt_sn::SubscribeAck subscribe_ack;
    subscribe_ack.topic_id = 1;
    subscribe_ack.message_id = 2;
    subscribe_ack.code = mqtt_sn::MessageErrorCode::Accepted;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(subscribe_ack, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::SubscribeAck>(msg.value()));
}

TEST_CASE("Unsubscribe", "[format]") {
    mqtt_sn::Unsubscribe unsubscribe;
    unsubscribe.flags.topic_id_type = static_cast<uint8_t>(mqtt_sn::TopicIdType::Normal);
    unsubscribe.message_id = 1;
    unsubscribe.topic = "foo";

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(unsubscribe, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Unsubscribe>(msg.value()));
}

TEST_CASE("Unsubscribe2", "[format]") {
    mqtt_sn::Unsubscribe unsubscribe;
    unsubscribe.flags.topic_id_type = static_cast<uint8_t>(mqtt_sn::TopicIdType::PreDefined);
    unsubscribe.message_id = 1;
    unsubscribe.topic = 2;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(unsubscribe, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Unsubscribe>(msg.value()));
}

TEST_CASE("UnsubscribeAck", "[format]") {
    mqtt_sn::UnsubscribeAck unsubscribe_ack;
    unsubscribe_ack.message_id = 1;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(unsubscribe_ack, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::UnsubscribeAck>(msg.value()));

    REQUIRE(std::get<mqtt_sn::UnsubscribeAck>(msg.value()).message_id == 1);
}

TEST_CASE("PingRequest", "[format]") {
    mqtt_sn::PingRequest ping_request;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(ping_request, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PingRequest>(msg.value()));
}

TEST_CASE("PingRequest2", "[format]") {
    mqtt_sn::PingRequest ping_request;
    ping_request.client_id = "foo";

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(ping_request, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PingRequest>(msg.value()));

    REQUIRE(std::get<mqtt_sn::PingRequest>(msg.value()).client_id.has_value());
    REQUIRE(std::get<mqtt_sn::PingRequest>(msg.value()).client_id.value() == "foo");
}

TEST_CASE("PingResponse", "[format]") {
    mqtt_sn::PingResponse ping_response;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(ping_response, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::PingResponse>(msg.value()));
}

TEST_CASE("Disconnect", "[format]") {
    mqtt_sn::Disconnect disconnect;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(disconnect, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Disconnect>(msg.value()));
}

TEST_CASE("Disconnect2", "[format]") {
    mqtt_sn::Disconnect disconnect;
    disconnect.duration = 1;

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(disconnect, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Disconnect>(msg.value()));

    auto& disconnect_msg = std::get<mqtt_sn::Disconnect>(msg.value());
    REQUIRE(disconnect_msg.duration == 1);
}

TEST_CASE("Forward", "[format]") {
    mqtt_sn::Forward forward;
    forward.ctrl = 1 & mqtt_sn::FORWARD_CTRL_RADIUS_MASK;
    forward.gateway_addr = std::vector<uint8_t> {1, 2, 3};
    forward.payload = std::vector<uint8_t> {1, 2, 3};

    mqtt_sn::format::BufferWriter buffer;
    mqtt_sn::format::encode(forward, buffer);

    auto reader = mqtt_sn::format::BufferReader(buffer.data(), buffer.size());
    auto msg = mqtt_sn::format::parse(reader);
    
    REQUIRE(msg.has_value());
    REQUIRE(std::holds_alternative<mqtt_sn::Forward>(msg.value()));

    auto& forward_msg = std::get<mqtt_sn::Forward>(msg.value());
    REQUIRE(forward_msg.ctrl == 1);
    REQUIRE(forward_msg.gateway_addr == std::vector<uint8_t> {1, 2, 3});
    REQUIRE(forward_msg.payload == std::vector<uint8_t> {1, 2, 3});
}