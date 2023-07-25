#pragma once

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include "baggage/http_filter.pb.h"


namespace Envoy {
namespace Http {

class HttpBaggageFilterConfig {
public:
    HttpBaggageFilterConfig(const baggage::Baggage& proto_config);

    const std::vector<std::string>& metadata_keys() const { return metadata_keys_; }

private:
    const std::vector<std::string> metadata_keys_;
}; 

using HttpBaggageFilterConfigSharedPtr = std::shared_ptr<HttpBaggageFilterConfig>;

class HttpBaggageFilter: public PassThroughDecoderFilter {
    public:
        HttpBaggageFilter(HttpBaggageFilterConfigSharedPtr);
        ~HttpBaggageFilter();

        // Http::StreamDecoderFilter
        void onDestroy() override;

        FilterHeadersStatus decodeHeaders(RequestHeaderMap&, bool) override;
        FilterDataStatus decodeData(Buffer::Instance&, bool) override;
        void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;
    private:
        HttpBaggageFilterConfigSharedPtr config_;
};

} // namespace Http
} // namespace Envoy
