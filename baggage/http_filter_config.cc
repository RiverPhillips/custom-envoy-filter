#include <string>

#include "envoy/server/filter_config.h"
#include "envoy/registry/registry.h"

#include "baggage/http_filter.pb.h"
#include "baggage/http_filter.pb.validate.h"
#include "http_filter.h"


namespace Envoy {
namespace Server {
namespace Configuration{

class HttpSampleDecoderFilterConfigFactory : public NamedHttpFilterConfigFactory {
public:
    Http::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message& proto_config,
                                                       const std::string&,
                                                       FactoryContext& context) override {
        return createFilter(Envoy::MessageUtil::downcastAndValidate<const baggage::Baggage&>(proto_config, context.messageValidationVisitor()),
                            context);
    }

    ProtobufTypes::MessagePtr createEmptyConfigProto() override {
        return ProtobufTypes::MessagePtr{new baggage::Baggage()};
    }

    std::string name() const override { return "baggage"; }

private:
    Http::FilterFactoryCb createFilter(const baggage::Baggage& proto_config, FactoryContext&) {
        Http::HttpBaggageFilterConfigSharedPtr config 
            = std::make_shared<Http::HttpBaggageFilterConfig>(
                Http::HttpBaggageFilterConfig(proto_config));

        return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
            auto filter = new Http::HttpBaggageFilter(config);
            callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{filter});
        };
    }
};

static Registry::RegisterFactory<HttpSampleDecoderFilterConfigFactory, NamedHttpFilterConfigFactory> register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy