# GridIron

C++ "Code-Beside" framework, modeled after ASP.NET, based on Oat++

See more:

- [GridIron Wiki](https://github.com/ProjectGridIron/GridIron/wiki)
- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
- [Oat++ Getting Started Guide](https://oatpp.io/docs/start)

## Overview

This project is using [oatpp](https://github.com/oatpp/oatpp) and [oatpp-swagger](https://github.com/oatpp/oatpp-swagger) modules.

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- include/
|   |
|   |- gridiron/                         // GridIron header files
|   |   |- base_classes/                 // GridIron headers for core classes
|   |       |- controls/                 // GridIron headers for custom controls
|   |           |- ui/                   // GridIron headers for custom ui controls
| 
|- src/
|   |
|   |- controller/                       // Folder containing UserController where all endpoints are declared
|   |- db/                               // Folder with database mock
|   |- dto/                              // DTOs are declared here
|   |- gridiron/                         // GridIron library root
|   |    |- html/                        // GridIron HTML root for C++ pages
|   |        |- base_classes/            // GridIron core library
|   |            |- controls/            // GridIron Custom controls root
|   |                |- ui/              // UI controls eg <gi:label>
|   |- SwaggerComponent.hpp              // Swagger-UI config
|   |- AppComponent.hpp                  // Service config
|   |- App.cpp                           // main() is here
|
|- test/                                 // test folder
|- utility/install-oatpp-modules.sh      // utility script to install required oatpp-modules.
```

---

### Build and Run

#### Using CMake

**Requires**

- `oatpp` and `oatpp-swagger` modules installed. You may run `utility/install-oatpp-modules.sh` 
script to install required oatpp modules.

```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./gridiron-demo        # - run application.
```

#### In Docker

```
$ docker build -t example-crud .
$ docker run -p 8000:8000 -t example-crud
```

---

### Example endpoint boilerplate (*the render mechanism is about to change)
```
  ENDPOINT("GET", "/", root) {
    GridIron::Page page("gridiron-demo/testapp.html");
    GridIron::controls::Label lblTest("lblTest", &page);
    page.RegisterVariable("lblTest_Text", lblTest.GetTextPtr());
    lblTest.SetText("these contents were replaced");
    auto response = createResponse(Status::CODE_200, page.render().c_str());
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }
```

### "Code-Beside"
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<GridIron::Page lang="en">
<head>
    <meta charset="utf-8" />
    <title>GridIron Test Page</title>
</head>
<body bgcolor="#ffffff" text="#000000">
<h2>GridIron Test Page</h2>
<hr/>
<table border="0" cellpadding="5" cellspacing="5">
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">Variable Replacement:</th>
    </tr>
    <tr>
        <td align="left"><b>Front Page:</b></td>
        <td align="left"><GridIron::Value key="gridiron.frontPage" /></td>
    </tr>
    <tr>
        <td align="left"><b>Front Page File:</b></td>
        <td align="left"><GridIron::Value key="gridiron.frontPageFile /></td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">GridIron::Label Test<br>
            <small>This will show "default contents" if SetText did not work</small></th>
    </tr>
    <tr>
        <td align="left" colspan="2">
            <GridIron::Label id="lblTest">default contents</GridIron::Label>
        </td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">Variable Replacement with text from lblTest:</th>
    </tr>
    <tr>
        <td align="left"><b>lblTest.Text:</b></td>
        <td align="left"><GridIron::Value key="lblTest.Text" /></td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">Autonomous GridIron::Label Test<br>
            <small>This should show "default auto contents".</small></th>
    </tr>
    <tr>
        <td align="left" colspan="2">
            <GridIron::Label auto="true" id="lblAutoTest">default auto contents</GridIron::Label>
        </td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">Variable Replacement with text from lblAutoTest:</th>
    </tr>
    <tr>
        <td align="left"><b>lblAutoTest.Text:</b></td>
        <td align="left"><GridIron::Value key="lblAutoTest.Text" /></td>
    </tr>
</table>
<a href="swagger/ui">Checkout Swagger-UI page</a>
</body>
</GridIron::Page>
```
### Sample output/test-page
![](https://raw.githubusercontent.com/ProjectGridIron/GridIron/master/github-assets/gridiron-demo.png)
