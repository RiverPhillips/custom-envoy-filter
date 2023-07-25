#include "test/integration/http_integration.h"

namespace Envoy {
    class HttpFilterBaggageIntegrationTest: public HttpIntegrationTest, public testing::TestWithParam<Network::Address::IpVersion> {

    };

}// namespace Envoys