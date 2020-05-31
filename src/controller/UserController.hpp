/****************************************************************************************
   * (C) Copyright 2009-2020
   *    Jessica Mulein <jessica@mulein.com>
   *
   * Others will be credited if more developers join.
   *
   * License
   *
   * This code is licensed under the GPLv3.
   * Please see COPYING in the root of this package for details.
   *
   * The following libraries are only linked in, and no code is based directly from them:
   * htmlcxx is under the Apache 2.0 License
   * Boost is under the Boost license
   ***************************************************************************************/

#ifndef UserController_hpp
#define UserController_hpp

#include "../db/Database.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <gridiron/base_classes/page.hpp>
#include <gridiron/base_classes/controls/ui/label.hpp>
// standard
#include <iostream>
#include <string>
#include <fstream>
// htmlcxx

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class UserController : public oatpp::web::server::api::ApiController {
public:
  UserController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  
  /**
   *  Inject Database component
   */
  OATPP_COMPONENT(std::shared_ptr<Database>, m_database);
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<UserController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                 objectMapper)){
    return std::make_shared<UserController>(objectMapper);
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Insert Your endpoints here !!!
   */
  
  ENDPOINT_INFO(root) {
    info->summary = "Index.html page";
    info->addResponse<String>(Status::CODE_200, "text/html");
  }
  ENDPOINT("GET", "/", root) {

    GridIron::Page page("crud-exe/testapp.html");
    GridIron::controls::Label lblTest("lblTest", &page);
    page.RegisterVariable("lblTest_Text", lblTest.GetTextPtr());

    lblTest.SetText("these contents were replaced");

    //page.render(stream)
    auto response = createResponse(Status::CODE_200, "");
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }
  
  ENDPOINT_INFO(createUser) {
    info->summary = "Create new User";
    info->addConsumes<Object<UserDto>>("application/json");
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "demo/api/users", createUser,
           BODY_DTO(Object<UserDto>, userDto)) {
    return createDtoResponse(Status::CODE_200, m_database->createUser(userDto));
  }
  
  
  ENDPOINT_INFO(putUser) {
    // general
    info->summary = "Update User by userId";
    info->addConsumes<Object<UserDto>>("application/json");
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["userId"].description = "User Identifier";
  }
  ENDPOINT("PUT", "demo/api/users/{userId}", putUser,
           PATH(Int32, userId),
           BODY_DTO(Object<UserDto>, userDto)) {
    userDto->id = userId;
    return createDtoResponse(Status::CODE_200, m_database->updateUser(userDto));
  }
  
  
  ENDPOINT_INFO(getUserById) {
    // general
    info->summary = "Get one User by userId";
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["userId"].description = "User Identifier";
  }
  ENDPOINT("GET", "demo/api/users/{userId}", getUserById,
           PATH(Int32, userId)) {
    auto user = m_database->getUserById(userId);
    OATPP_ASSERT_HTTP(user, Status::CODE_404, "User not found");
    return createDtoResponse(Status::CODE_200, user);
  }
  
  
  ENDPOINT_INFO(getUsers) {
    info->summary = "get all stored users";
    info->addResponse<List<Object<UserDto>>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "demo/api/users", getUsers) {
    return createDtoResponse(Status::CODE_200, m_database->getUsers());
  }
  
  
  ENDPOINT_INFO(deleteUser) {
    // general
    info->summary = "Delete User by userId";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["userId"].description = "User Identifier";
  }
  ENDPOINT("DELETE", "demo/api/users/{userId}", deleteUser,
           PATH(Int32, userId)) {
    bool success = m_database->deleteUser(userId);
    OATPP_ASSERT_HTTP(success, Status::CODE_417, "User not deleted. Perhaps no such User in the Database");
    return createResponse(Status::CODE_200, "User successfully deleted");
  }
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* UserController_hpp */
