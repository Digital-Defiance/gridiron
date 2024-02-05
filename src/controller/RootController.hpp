
#ifndef RootController_hpp
#define RootController_hpp

#include <sstream>
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include <gridiron/gridiron.hpp>
#include <gridiron/controls/ui/label.hpp>
#include <gridiron/controls/page.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin codegen

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class RootController : public oatpp::web::server::api::ApiController
{
protected:
    RootController(const std::shared_ptr<ObjectMapper> &objectMapper)
        : oatpp::web::server::api::ApiController(objectMapper)
    {
    }

public:
    /**
     *  Inject @objectMapper component here as default parameter
     *  Do not return bare Controllable* object! use shared_ptr!
     */
    static std::shared_ptr<RootController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                        objectMapper))
    {
        return std::shared_ptr<RootController>(new RootController(objectMapper));
    }

    /**
     *  Hello World endpoint Coroutine mapped to the "/" (root)
     */
    ENDPOINT_ASYNC("GET", "/", Root){
        ENDPOINT_ASYNC_INIT(Root)

            Action act() override{

            auto page = std::make_shared<GridIron::Page>("gridiron-demo/testapp.html");
            GridIron::controls::Label lblTest("lblTest", page);

            page->RegisterVariable("lblTest_Text", lblTest.GetTextPtr());
            lblTest.SetText("these contents were replaced");

            std::ostringstream pageContent;
            pageContent << page;
            auto response = controller->createResponse(Status::CODE_200, pageContent.str().c_str());
            response->putHeader("Content-Type", "text/html");
            return _return(response);
        }
    }
;
}
;

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- End codegen

#endif /* RootController_hpp */