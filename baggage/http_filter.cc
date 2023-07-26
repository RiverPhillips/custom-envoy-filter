#include "http_filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {

HttpBaggageFilterConfig::HttpBaggageFilterConfig(
    const baggage::Baggage& proto_config) : metadata_keys_({proto_config.metadata_keys().begin(), proto_config.metadata_keys().end()}) {}

HttpBaggageFilter::HttpBaggageFilter(HttpBaggageFilterConfigSharedPtr config): config_(config) {}

HttpBaggageFilter::~HttpBaggageFilter() {}

void HttpBaggageFilter::onDestroy() {}

FilterHeadersStatus HttpBaggageFilter::decodeHeaders(RequestHeaderMap&, bool) {
  const auto& metadata = decoder_callbacks_->streamInfo().dynamicMetadata().filter_metadata();
  
  // For each metadata key, check if it exists in the metadata map. If it does add it to the request Baggage
  for (const auto& key : config_->metadata_keys()) {
    const auto& metadata_it = metadata.find(key);
    if (metadata_it != metadata.end()) {
      const auto pb_struct = metadata_it->second;

      const auto& clientId = pb_struct.fields().at("client_id").string_value();

      decoder_callbacks_->activeSpan().setBaggage(key, clientId);
    }
  }


  return FilterHeadersStatus::Continue;
}

FilterDataStatus HttpBaggageFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

void HttpBaggageFilter::setDecoderFilterCallbacks(
    StreamDecoderFilterCallbacks&) {}

} // namespace Http
} // namespace Envoy
