#include <cstdint>
#include <mqtt-sn/format.h>

#include <cassert>

#define assertm(exp, msg) assert((void(msg), exp))

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloads(Ts...) -> overloads<Ts...>;

namespace mqtt_sn::format {

optional<Message> parse(BufferReader& buffer) {
    if (buffer.readable_bytes() < 2) {
        return nullopt;
    }

    auto base_offset = buffer.read_offset();
    uint16_t len = buffer.read<uint8_t>().value();
    if (len == 1) {
        if (buffer.readable_bytes() < sizeof(uint8_t) + sizeof(uint16_t)) {
            return nullopt;
        }

        len = buffer.read<uint16_t>().value();   
    }
    
    if (buffer.size() - base_offset < len) {
        return nullopt;
    }

    auto type = buffer.read<uint8_t>();
    if (!type) {
        return nullopt;
    }

    if (len - (buffer.read_offset() - base_offset) > buffer.readable_bytes()) {
        return nullopt;
    }

    MessageType msg_type = static_cast<MessageType>(type.value());
    switch (msg_type) {
        case MessageType::Advertise:
            return Advertise {buffer.read<uint8_t>().value(), buffer.read<uint16_t>().value()};
        case MessageType::SearchGateway:
            return SearchGateway {buffer.read<uint8_t>().value()};
        case MessageType::GatewayInfo: 
            if (sizeof(uint8_t) == len - (buffer.read_offset() - base_offset)) {
                return GatewayInfo {buffer.read<uint8_t>().value(), nullopt};
            }

            return GatewayInfo {buffer.read<uint8_t>().value(),
                    buffer.read<vector<uint8_t>>(len - (buffer.read_offset() - base_offset))};
        


        case MessageType::Connect: {
            return Connect {
                buffer.read<MessageFlags>().value(),
                buffer.read<uint8_t>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<std::string>(len - (buffer.read_offset() - base_offset)).value()
            };
        }
        case MessageType::ConnectAck:
            return ConnectAck {
                static_cast<MessageErrorCode>(buffer.read<uint8_t>().value())
            };
        
        
        
        case MessageType::WillTopicRequest:
            return WillTopicRequest {};
        case MessageType::WillTopic: 
            if (len - (buffer.read_offset() - base_offset) < 1) {
                return WillTopicEmpty {};
            }
            return WillTopic {
                buffer.read<MessageFlags>().value(),
                buffer.read<std::string>(len - (buffer.read_offset() - base_offset)).value()
            };
        case MessageType::WillMessageRequest:
            return WillMessageRequest {};
        case MessageType::WillMessage:
            return WillMessage {
                buffer.read<vector<uint8_t>>(len - (buffer.read_offset() - base_offset)).value()
            };
        case MessageType::WillTopicUpdate:
            if (len - (buffer.read_offset() - base_offset) < 1) {
                return WillTopicUpdateEmpty {};
            }

            return WillTopicUpdate {
                buffer.read<MessageFlags>().value(),
                buffer.read<std::string>(len - (buffer.read_offset() - base_offset)).value()
            };
        case MessageType::WillTopicResponse:
            return WillTopicResponse {
                buffer.read<MessageErrorCode>().value()
            };
        case MessageType::WillMessageUpdate: {
            return WillMessageUpdate {
                buffer.read<vector<uint8_t>>(len - (buffer.read_offset() - base_offset)).value()
            };
        }
        case MessageType::WillMessageResponse:
            return WillMessageResponse {
                buffer.read<MessageErrorCode>().value()
            };
        

        
        case MessageType::Register:
            return RegisterTopic {
                buffer.read<uint16_t>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<std::string>(len - (buffer.read_offset() - base_offset)).value()
            };
        case MessageType::RegisterAck:
            return RegisterTopicAck {
                buffer.read<uint16_t>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<MessageErrorCode>().value()
            };
        

        
        case MessageType::Publish:
            return PublishMessage {
                buffer.read<MessageFlags>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<vector<uint8_t>>(len - (buffer.read_offset() - base_offset)).value()
            };
        case MessageType::PublishAck:
            return PublishMessageAck {
                buffer.read<uint16_t>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<MessageErrorCode>().value()
            };
        case MessageType::PublishComplete:
            return PublishMessageComplete {
                buffer.read<uint16_t>().value()
            };
        case MessageType::PublishReceived:
            return PublishMessageReceived {
                buffer.read<uint16_t>().value()
            };
        case MessageType::PublishRelease:
            return PublishMessageRelease {
                buffer.read<uint16_t>().value()
            };
        

        
        case MessageType::Subscribe: {
            auto sub = Subscribe {
                buffer.read<MessageFlags>().value(),
                buffer.read<uint16_t>().value(),
                uint16_t(0)
            };
            auto topic_type = static_cast<TopicIdType>(sub.flags.topic_id_type);
            if (topic_type == TopicIdType::Normal) {
                sub.topic = buffer.read<std::string>(len - (buffer.read_offset() - base_offset)).value();
            } else {
                sub.topic = buffer.read<uint16_t>().value();
            }
            return sub;
        }
        case MessageType::SubscribeAck:
            return SubscribeAck {
                buffer.read<MessageFlags>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<uint16_t>().value(),
                buffer.read<MessageErrorCode>().value()
            };
        case MessageType::Unsubscribe: {
            auto sub = Unsubscribe {
                buffer.read<MessageFlags>().value(),
                buffer.read<uint16_t>().value(),
                0
            };
            auto topic_type = static_cast<TopicIdType>(sub.flags.topic_id_type);
            if (topic_type == TopicIdType::Normal) {
                sub.topic = buffer.read<std::string>(len - (buffer.read_offset() - base_offset)).value();
            } else {
                sub.topic = buffer.read<uint16_t>().value();
            }
            return sub;
        }
        case MessageType::UnsubscribeAck:
            return UnsubscribeAck {
                buffer.read<uint16_t>().value()
            };
        
        

        case MessageType::PingRequest:
            if (len - (buffer.read_offset() - base_offset) == 0) {
                return PingRequest {};
            }
            return PingRequest {
                buffer.read<std::string>(len - (buffer.read_offset() - base_offset)).value()
            };
        case MessageType::PingResponse:
            return PingResponse {};
        case MessageType::Disconnect:
            if (len - (buffer.read_offset() - base_offset) == 0) {
                return Disconnect {};
            }
            return Disconnect {
                buffer.read<uint16_t>().value()
            };
        case MessageType::Forward:
            return Forward {
                buffer.read<uint8_t>().value(),
                buffer.read<vector<uint8_t>>(len - (buffer.read_offset() - base_offset)).value(),
                buffer.read<vector<uint8_t>>(buffer.readable_bytes()).value()
            };
        default:
            return nullopt;
    }
}

void encode(const Message& message, BufferWriter& buffer) {
    auto base_offset = buffer.size();
    const auto visitor = overloads {
        [&](const Advertise& n){
            buffer.write(static_cast<uint8_t>(5));
            buffer.write(MessageType::Advertise);
            buffer.write(n.gateway_id);
            buffer.write(n.duration);

            assertm(buffer.size() - base_offset == 5, "Advertise must be 5 bytes long");
        },
        [&](const SearchGateway& n){
            buffer.write(static_cast<uint8_t>(3));
            buffer.write(MessageType::SearchGateway);
            buffer.write(n.radius);

            assertm(buffer.size() - base_offset == 3, "SearchGateway must be 3 bytes long");
        },
        [&](const GatewayInfo& n){
            auto len = static_cast<uint8_t>(3) + (n.gateway_addr ? n.gateway_addr->size() : 0);
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::GatewayInfo);
            buffer.write(n.gateway_id);
            if (n.gateway_addr) {
                buffer.write(n.gateway_addr.value());
            }

            assertm(len < 256, "GatewayInfo must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "GatewayInfo must be " + std::to_string(len) + " bytes long");
        },
        [&](const Connect& n){
            auto len = static_cast<uint8_t>(6) + n.client_id.size();
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::Connect);
            buffer.write(n.flags);
            buffer.write(n.protocol_version);
            buffer.write(n.duration);
            buffer.write(n.client_id);

            assertm(len < 256, "Connect must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "Connect must be " + std::to_string(len) + " bytes long");
        },
        [&](const ConnectAck& n){
            buffer.write(static_cast<uint8_t>(3));
            buffer.write(MessageType::ConnectAck);
            buffer.write(n.code);

            assertm(buffer.size() - base_offset == 3, "ConnectAck must be 3 bytes long");
        },
        [&](const WillTopicRequest& n){
            buffer.write(static_cast<uint8_t>(2));
            buffer.write(MessageType::WillTopicRequest);

            assertm(buffer.size() - base_offset == 2, "WillTopicRequest must be 2 bytes long");
        },
        [&](const WillTopicEmpty&){
            buffer.write(static_cast<uint8_t>(2));
            buffer.write(MessageType::WillTopic);

            assertm(buffer.size() - base_offset == 2, "WillTopicEmpty must be 2 bytes long");
        },
        [&](const WillTopic& n){
            auto len = static_cast<uint8_t>(3) + n.topic.size();
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::WillTopic);
            buffer.write(n.flags);
            buffer.write(n.topic);

            assertm(len < 256, "WillTopic must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "WillTopic must be " + std::to_string(len) + " bytes long");
        },
        [&](const WillMessageRequest&){
            buffer.write(static_cast<uint8_t>(2));
            buffer.write(MessageType::WillMessageRequest);

            assertm(buffer.size() - base_offset == 2, "WillMessageRequest must be 2 bytes long");
        },
        [&](const WillMessage& n){
            auto len = static_cast<uint8_t>(2) + n.payload.size();
            if (len < 256) {
                buffer.write(static_cast<uint8_t>(len));
            } else {
                buffer.write(static_cast<uint8_t>(1));
                len += + sizeof(uint16_t);
                buffer.write(static_cast<uint16_t>(len));
            }
            buffer.write(MessageType::WillMessage);
            buffer.write(n.payload);

            assertm(len <= 65535, "WillMessage must be less than 65535 bytes long");
            assertm(buffer.size() - base_offset == len, "WillMessage must be " + std::to_string(len) + " bytes long");
        },
        [&](const WillTopicUpdate& n){
            auto len = static_cast<uint8_t>(3) + n.topic.size();
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::WillTopicUpdate);
            buffer.write(n.flags);
            buffer.write(n.topic);

            assertm(len < 256, "WillTopicUpdate must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "WillTopicUpdate must be " + std::to_string(len) + " bytes long");
        },
        [&](const WillTopicUpdateEmpty&){
            buffer.write(static_cast<uint8_t>(2));
            buffer.write(MessageType::WillTopicUpdate);

            assertm(buffer.size() - base_offset == 2, "WillTopicUpdateEmpty must be 2 bytes long");
        },
        [&](const WillTopicResponse& n){
            buffer.write(static_cast<uint8_t>(3));
            buffer.write(MessageType::WillTopicResponse);
            buffer.write(n.code);

            assertm(buffer.size() - base_offset == 3, "WillTopicResponse must be 3 bytes long");
        },
        [&](const WillMessageUpdate& n){
            auto len = static_cast<uint8_t>(2) + n.payload.size();
            if (len < 256) {
                buffer.write(static_cast<uint8_t>(len));
            } else {
                buffer.write(static_cast<uint8_t>(1));
                len += + sizeof(uint16_t);
                buffer.write(static_cast<uint16_t>(len));
            }
            buffer.write(MessageType::WillMessageUpdate);
            buffer.write(n.payload);

            assertm(len <= 65535, "WillMessageUpdate must be less than or equal to 65535 bytes long");
            assertm(buffer.size() - base_offset == len, "WillMessageUpdate must be " + std::to_string(len) + " bytes long");
        },
        [&](const WillMessageResponse& n){
            buffer.write(static_cast<uint8_t>(3));
            buffer.write(MessageType::WillMessageResponse);
            buffer.write(n.code);

            assertm(buffer.size() - base_offset == 3, "WillMessageResponse must be 3 bytes long");
        },
        [&](const RegisterTopic& n){
            auto len = static_cast<uint8_t>(6) + n.topic.size();
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::Register);
            buffer.write(n.topic_id);
            buffer.write(n.message_id);
            buffer.write(n.topic);

            assertm(len < 256, "RegisterTopic must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "RegisterTopic must be " + std::to_string(len) + " bytes long");
        },
        [&](const RegisterTopicAck& n){
            buffer.write(static_cast<uint8_t>(7));
            buffer.write(MessageType::RegisterAck);
            buffer.write(n.topic_id);
            buffer.write(n.message_id);
            buffer.write(n.code);

            assertm(buffer.size() - base_offset == 7, "RegisterTopicAck must be 7 bytes long");
        },
        [&](const PublishMessage& n){
            auto len = static_cast<uint8_t>(7) + n.payload.size();
            if (len < 256) {
                buffer.write(static_cast<uint8_t>(len));
            } else {
                buffer.write(static_cast<uint8_t>(1));
                len += + sizeof(uint16_t);
                buffer.write(static_cast<uint16_t>(len));
            }
            buffer.write(MessageType::Publish);
            buffer.write(n.flags);
            buffer.write(n.topic_id);
            buffer.write(n.message_id);
            buffer.write(n.payload);

            assertm(len <= 65535, "PublishMessage must be less than or equal to 65535 bytes long");
            assertm(buffer.size() - base_offset == len, "PublishMessage must be " + std::to_string(len) + " bytes long");
        },
        [&](const PublishMessageAck& n){
            buffer.write(static_cast<uint8_t>(7));
            buffer.write(MessageType::PublishAck);
            buffer.write(n.topic_id);
            buffer.write(n.message_id);
            buffer.write(n.code);

            assertm(buffer.size() - base_offset == 7, "PublishMessageAck must be 7 bytes long");
        },
        [&](const PublishMessageComplete& n){
            buffer.write(static_cast<uint8_t>(4));
            buffer.write(MessageType::PublishComplete);
            buffer.write(n.message_id);

            assertm(buffer.size() - base_offset == 4, "PublishMessageComplete must be 4 bytes long");
        },
        [&](const PublishMessageReceived& n){
            buffer.write(static_cast<uint8_t>(4));
            buffer.write(MessageType::PublishReceived);
            buffer.write(n.message_id);

            assertm(buffer.size() - base_offset == 4, "PublishMessageReceived must be 4 bytes long");
        },
        [&](const PublishMessageRelease& n){
            buffer.write(static_cast<uint8_t>(4));
            buffer.write(MessageType::PublishRelease);
            buffer.write(n.message_id);

            assertm(buffer.size() - base_offset == 4, "PublishMessageRelease must be 4 bytes long");
        },
        [&](const Subscribe& n){
            auto is_topic_id = std::holds_alternative<uint16_t>(n.topic);
            auto len = static_cast<uint8_t>(5)
                    + (is_topic_id ? sizeof(uint16_t) : std::get<std::string>(n.topic).size());
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::Subscribe);
            buffer.write(n.flags);
            buffer.write(n.message_id);
            if (is_topic_id) {
                buffer.write(std::get<uint16_t>(n.topic));
            } else {
                buffer.write(std::get<std::string>(n.topic));
            }

            assertm(len < 256, "Subscribe must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "Subscribe must be " + std::to_string(len) + " bytes long");
        },
        [&](const SubscribeAck& n){
            buffer.write(static_cast<uint8_t>(8));
            buffer.write(MessageType::SubscribeAck);
            buffer.write(n.flags);
            buffer.write(n.topic_id);
            buffer.write(n.message_id);
            buffer.write(n.code);

            assertm(buffer.size() - base_offset == 8, "SubscribeAck must be 8 bytes long");
        },
        [&](const Unsubscribe& n){
            auto is_topic_id = std::holds_alternative<uint16_t>(n.topic);
            auto len = static_cast<uint8_t>(5)
                    + (is_topic_id ? sizeof(uint16_t) : std::get<std::string>(n.topic).size());
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::Unsubscribe);
            buffer.write(n.flags);
            buffer.write(n.message_id);
            if (is_topic_id) {
                buffer.write(std::get<uint16_t>(n.topic));
            } else {
                buffer.write(std::get<std::string>(n.topic));
            }

            assertm(len < 256, "Unsubscribe must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "Unsubscribe must be " + std::to_string(len) + " bytes long");
        },
        [&](const UnsubscribeAck& n){
            buffer.write(static_cast<uint8_t>(4));
            buffer.write(MessageType::UnsubscribeAck);
            buffer.write(n.message_id);

            assertm(buffer.size() - base_offset == 4, "UnsubscribeAck must be 4 bytes long");
        },
        [&](const PingRequest& n){
            auto len = static_cast<uint8_t>(2)
                    + (n.client_id ? n.client_id->size() : 0);
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::PingRequest);
            if (n.client_id) {
                buffer.write(n.client_id.value());
            }

            assertm(len < 256, "PingRequest must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "PingRequest must be " + std::to_string(len) + " bytes long");
        },
        [&](const PingResponse& n){
            buffer.write(static_cast<uint8_t>(2));
            buffer.write(MessageType::PingResponse);

            assertm(buffer.size() - base_offset == 2, "PingResponse must be 2 bytes long");
        },
        [&](const Disconnect& n){
            auto len = static_cast<uint8_t>(2) + (n.duration ? sizeof(uint16_t) : 0);
            buffer.write(static_cast<uint8_t>(len));
            buffer.write(MessageType::Disconnect);
            if (n.duration) {
                buffer.write(n.duration.value());
            }

            assertm(len < 256, "Disconnect must be less than 256 bytes long");
            assertm(buffer.size() - base_offset == len, "Disconnect must be " + std::to_string(len) + " bytes long");
        },
        [&](const Forward& n){
            auto len = static_cast<uint8_t>(3)
                    + n.gateway_addr.size();
            
            if (len < 256) {
                buffer.write(static_cast<uint8_t>(len));
            } else {
                buffer.write(static_cast<uint8_t>(1));
                len += + sizeof(uint16_t);
                buffer.write(static_cast<uint16_t>(len));
            }
            
            buffer.write(MessageType::Forward);
            buffer.write(n.ctrl);
            buffer.write(n.gateway_addr);
            buffer.write(n.payload);

            assertm(len < 65535, "Forward must be less than 65535 bytes long");
            assertm(buffer.size() - n.payload.size() - base_offset == len, "Forward must be " + std::to_string(len) + " bytes long");
        }
    };

    std::visit(visitor, message);
}

}