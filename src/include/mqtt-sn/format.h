#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include <string>
#include <variant>

namespace mqtt_sn {

using std::optional, std::nullopt;
using std::vector;

enum class MessageErrorCode : uint8_t {
    Accepted = 0x00,
    Congestion = 0x01,
    InvalidTopicId = 0x02,
    NotSupported = 0x03,
    Unknown = 0x04
};

enum class TopicIdType : uint8_t {
    Normal = 0x00,
    PreDefined = 0x01,
    Short = 0x02
};

/**
 * @brief Defines the message types for the communication protocol.
 *
 * This enumeration class specifies the various types of messages that can be
 * exchanged between clients and the broker. Using a uint8_t as the underlying
 * type is efficient for network transmission.
 */
enum class MessageType : uint8_t {
    // Discovery and Session Setup
    Advertise         = 0x00,
    SearchGateway     = 0x01,
    GatewayInfo       = 0x02,

    // Connection
    Connect           = 0x04,
    ConnectAck        = 0x05,

    // Last Will and Testament (LWT)
    WillTopicRequest  = 0x06,
    WillTopic         = 0x07,
    WillMessageRequest= 0x08,
    WillMessage       = 0x09,
    WillTopicUpdate   = 0x1A,
    WillTopicResponse = 0x1B,
    WillMessageUpdate = 0x1C,
    WillMessageResponse= 0x1D,

    // Registration
    Register          = 0x0A,
    RegisterAck       = 0x0B,

    // Publish-Subscribe
    Publish           = 0x0C,
    PublishAck        = 0x0D, // QoS 1
    PublishComplete   = 0x0E, // QoS 2
    PublishReceived   = 0x0F, // QoS 2
    PublishRelease    = 0x10, // QoS 2

    Subscribe         = 0x12,
    SubscribeAck      = 0x13,
    Unsubscribe       = 0x14,
    UnsubscribeAck    = 0x15,

    // Keep-alive and Disconnection
    PingRequest       = 0x16,
    PingResponse      = 0x17,
    Disconnect        = 0x18,

    // Forwarding (for specific gateway/bridge logic)
    Forward           = 0xFE
};

struct MessageFlags {
    union {
        struct {
            uint8_t dup : 1;
            uint8_t qos : 2;
            uint8_t retain : 1;
            uint8_t will : 1;
            uint8_t clean_session : 1;
            uint8_t topic_id_type : 2;
        };
        uint8_t value;
    };
};

struct Advertise {
    uint8_t gateway_id;
    uint16_t duration;
};

struct SearchGateway {
    uint8_t radius;
};

struct GatewayInfo {
    uint8_t gateway_id;
    optional<vector<uint8_t>> gateway_addr;
};


struct Connect {
    MessageFlags flags;
    uint8_t protocol_version;
    uint16_t duration;
    std::string client_id;
};

struct ConnectAck {
    MessageErrorCode code;
};

struct WillTopicRequest {};

/***
 * An empty WILLTOPIC message is a WILLTOPIC message without Flags and WillTopic field (i.e. it is exactly
 * 2 octets long). It is used by a client to delete the Will topic and the Will message stored in the server, see Section 6.4.
 */
struct WillTopicEmpty {};
struct WillTopic {
    MessageFlags flags;
    std::string topic;
};

struct WillMessageRequest {};

struct WillMessage {
    vector<uint8_t> payload;
};

/***
 * An empty WILLTOPICUPD message is a WILLTOPICUPD message without Flags and WillTopic field (i.e.
 * it is exactly 2 octets long). It is used by a client to delete its Will topic and Will message stored in the GW/server
 */
struct WillTopicUpdateEmpty {};
struct WillTopicUpdate {
    MessageFlags flags;
    std::string topic;
};

struct WillTopicResponse {
    MessageErrorCode code;
};

struct WillMessageUpdate {
    vector<uint8_t> payload;
};

struct WillMessageResponse {
    MessageErrorCode code;
};

struct RegisterTopic {
    uint16_t topic_id;
    uint16_t message_id;
    std::string topic;
};

struct RegisterTopicAck {
    uint16_t topic_id;
    uint16_t message_id;
    MessageErrorCode code;
};

struct PublishMessage {
    MessageFlags flags;
    uint16_t topic_id; //contains the topic id value or the short topic name for which the data is published
    uint16_t message_id;
    vector<uint8_t> payload;
};

struct PublishMessageAck {
    uint16_t topic_id;
    uint16_t message_id;
    MessageErrorCode code;
};

struct PublishMessageComplete {
    uint16_t message_id;
};

struct PublishMessageReceived {
    uint16_t message_id;
};

struct PublishMessageRelease {
    uint16_t message_id;
};

struct Subscribe {
    MessageFlags flags;
    uint16_t message_id;
    std::variant<uint16_t, std::string> topic;
};

struct SubscribeAck {
    MessageFlags flags;
    uint16_t topic_id;
    uint16_t message_id;
    MessageErrorCode code;
};

struct Unsubscribe {
    MessageFlags flags;
    uint16_t message_id;
    std::variant<uint16_t, std::string> topic;
};

struct UnsubscribeAck {
    uint16_t message_id;
};

struct PingRequest {
    optional<std::string> client_id;
};

struct PingResponse {};

struct Disconnect {
    optional<uint16_t> duration;
};

static constexpr uint8_t FORWARD_CTRL_RADIUS_MASK = 0b11;
struct Forward {
    uint8_t ctrl;
    vector<uint8_t> gateway_addr;
    vector<uint8_t> payload;
};

using Message = std::variant<Advertise, SearchGateway, GatewayInfo, Connect, ConnectAck,
                             WillTopicRequest, WillTopicEmpty, WillTopic, WillMessageRequest, WillMessage,
                             WillTopicUpdate, WillTopicResponse, WillTopicUpdateEmpty, WillMessageUpdate, WillMessageResponse,
                             RegisterTopic, RegisterTopicAck, PublishMessage, PublishMessageAck, PublishMessageComplete,
                             PublishMessageReceived, PublishMessageRelease, Subscribe, SubscribeAck, Unsubscribe, UnsubscribeAck,
                             PingRequest, PingResponse, Disconnect, Forward>;



namespace format {
template<typename T>
struct is_vector : std::false_type {};

template<typename T>
struct is_vector<std::vector<T>> : std::true_type {};

class BufferReader {
public:
    BufferReader(const uint8_t* data, size_t size) : _data(data), _size(size) {}

    size_t size() const {
        return _size;
    }

    const uint8_t* data() const {
        return _data;
    }

    const uint8_t* begin() const {
        return _data;
    }

    const uint8_t* end() const {
        return _data + _size;
    }

    size_t readable_bytes() const {
        return _size - _read_offset;
    }

    template<typename T>
    bool readable() const {
        return readable_bytes() >= sizeof(T);
    }

    template<typename T>
    optional<T> read() {
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");

        if (readable_bytes() < sizeof(T)) {
            return nullopt;
        }

        T value;
        std::copy(this->begin() + _read_offset, this->begin() + _read_offset + sizeof(T), reinterpret_cast<uint8_t*>(&value));
        _read_offset += sizeof(T);
        return value;
    }

    template<typename T>
    optional<T> read(const size_t count) {
        if (count == 0) {
            return nullopt;
        }

        if constexpr (std::is_same<T, std::string>::value) {
            if (readable_bytes() < count) {
                return nullopt;
            }

            std::string value;
            value.resize(count);
            std::copy(this->begin() + _read_offset, this->begin() + _read_offset + count, value.begin());
            _read_offset += count;
            return value;
        } else if constexpr (is_vector<T>::value) {
            using ValueType = typename T::value_type;
            static_assert(std::is_trivially_copyable<ValueType>::value, "T must be trivially copyable");

            if (readable_bytes() < count * sizeof(ValueType)) {
                return nullopt;
            }

            T value;
            value.resize(count);
            std::copy(this->begin() + _read_offset, this->begin() + _read_offset + count * sizeof(ValueType), reinterpret_cast<uint8_t*>(value.data()));
            _read_offset += count * sizeof(ValueType);
            return value;
        } else {
            static_assert(false, "Unsupported type");
        }
    }

    void reset() {
        _read_offset = 0;
    }

    size_t read_offset() const {
        return _read_offset;
    }

    void skip(size_t offset) {
        _read_offset += offset;
    }

private:
    size_t _read_offset = 0;
    const uint8_t* _data;
    size_t _size;
};

class BufferWriter : public vector<uint8_t> {
public:
    template<typename T>
    void write(const T& value) {
        if constexpr (std::is_same<T, std::string>::value) {
            this->insert(this->end(), value.begin(), value.end());
        } else if constexpr (is_vector<T>::value) {
            using ValueType = typename T::value_type;
            static_assert(std::is_trivially_copyable<ValueType>::value, "T must be trivially copyable");

            this->insert(this->end(), reinterpret_cast<const uint8_t*>(value.data()), reinterpret_cast<const uint8_t*>(value.data()) + value.size() * sizeof(ValueType));
        } else {
            static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
            this->insert(this->end(), reinterpret_cast<const uint8_t*>(&value), reinterpret_cast<const uint8_t*>(&value) + sizeof(T));
        }
    }
};

optional<Message> parse(BufferReader& buffer);
void encode(const Message& message, BufferWriter& buffer);

}
}