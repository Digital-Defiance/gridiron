# Project GridIron
C++ XHTML "code-beside" Rendering framework with custom control classes similar to ASP.Net.

- GridIron itself is a self-contained rendering library with self-registering custom controls similar to ASP.NET.
- GridIron was created by Jessica Mulein in 2009 on SourceForge and was developed minimally through 2013. It was revived and retooled in May of 2020, during the COVID-19 Pandemic.
- GridIron, atop [Oat++](https://github.com/oatpp/oatpp), is a powerful content generation engine
- GridIron is an entirely separate project from Oat++, though the maintainer of Oat++ has been kind enough to offer development support
- An Oat++ integration layer/wrapper library is in https://github.com/ProjectGridIron/gridiron-oatpp
- A full GridIron+=(Oat++) stack example is in https://github.com/ProjectGridIron/gridiron-oatpp-example

## Links / See Also
- [GridIron Wiki](https://github.com/ProjectGridIron/gridiron/wiki) : Core Library
- [GridIron+=(Oat++)](https://github.com/ProjectGridIron/gridiron/gridiron-oatpp) : GridIron / Oat++ Integration Layer 
- [GridIron+=(Oat++) Example](https://github.com/ProjectGridIron/gridiron/gridiron-oatpp-example) : GridIron running on Oat++ with routing, etc
- [Oat++ GitHub](https://github.com/oatpp/oatpp) : Oat++ Source
- [Oat++ Website](https://oatpp.io) : Oat++ website with additional documentation and resources

## What Is It?
Oat++ is a C++ webserver/lightweight framework with a great many useful features like routing and authentication. Project GridIron is a dynamic custom UI/tag control system a bit like the ASP.Net codebehind style and with a structure to create custom controls.

## What does the name mean?
Grid(Network/The Internet) Iron("Big Iron"/Server/Mainframe)

## What is "code-beside"?
I made it up. ASP.Net (C#.Net/VB.Net/ASP Classic) is essentially XHTML frontends with functional programming backends. In this sense, Project GridIron is much the same, but based on C++ and the [Oat++ Framework](https://oatpp.io/). Project GridIron does require some minimal boilerplate code, though the pattern will hopefully get "DRY'd up" a little more soon. Interestingly though, Project GridIron brings something I call "autonomous controls" which are <gi:Label auto="true" id="lblTest">Value</gi:Label> instances that automatically create  instances that self-register with the Page and instantly allow another place in the XHTML to reference it by its ID, all without writing any backend code at all. <gi::Value key="lblTest.Text" /> would be automatically replaced with "Value". Chaining the value across multiple controls is not yet supported and the behavior will likely cause a crash or loop.
In summation, although some minimal boiler code is required to associate a page's C++ and XHTML files, the XHTML and C++ are essentially equal partners, hence "code-beside".

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- include/
|   |
|   |- gridiron/                     // GridIron headers for core classes
|   |   |- controls/                 // GridIron headers for custom controls
|   |       |- ui/                   // GridIron headers for custom ui controls
| 
|- src/
|   |
|   |- gridiron/                    // GridIron library root
|        |- html/                   // GridIron HTML root for C++ pages
|        |- controls/               // GridIron Custom controls root
|        |   |- ui/                 // UI controls eg <gi:label>
|        |
|        |- docs/                   // old documentation, TODO: update
|        |- test/                   // test folder
```

---

### Build and Run

#### Using CMake

```
$ mkdir cmake-build-test && cd cmake-build-test
$ cmake ..
```

**Requires**

- `htmlcxx` via homebrew, apt/yum

Example endpoint boilerplate (*the render mechanism is about to change)
```
  ENDPOINT("GET", "/", root) {
    GridIron::Page page("gridiron-demo/testapp.html");
    GridIron::controls::Label lblTest("lblTest", &page);
    lblTest.SetText("these contents were replaced");
    auto response = createResponse(Status::CODE_200, page.render().c_str());
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }
```

"Code-Beside"
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<gi::Page lang="en">
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
        <td align="left"><gi::Value key="gridiron.frontPage" /></td>
    </tr>
    <tr>
        <td align="left"><b>Front Page File:</b></td>
        <td align="left"><gi::Value key="gridiron.codeBesideFilename /></td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">gi::Label Test<br>
            <small>This will show "default contents" if Text did not work</small></th>
    </tr>
    <tr>
        <td align="left" colspan="2">
            <gi::Label id="lblTest">default contents</gi::Label>
        </td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">Variable Replacement with text from lblTest:</th>
    </tr>
    <tr>
        <td align="left"><b>lblTest.Text:</b></td>
        <td align="left"><gi::Value key="lblTest.Text" /></td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">Autonomous gi::Label Test<br>
            <small>This should show "default auto contents".</small></th>
    </tr>
    <tr>
        <td align="left" colspan="2">
            <gi::Label auto="true" id="lblAutoTest">default auto contents</gi::Label>
        </td>
    </tr>
    <tr>
        <th align="left" bgcolor="#dedede" colspan="2">Variable Replacement with text from lblAutoTest:</th>
    </tr>
    <tr>
        <td align="left"><b>lblAutoTest.Text:</b></td>
        <td align="left"><gi::Value key="lblAutoTest.Text" /></td>
    </tr>
</table>
</body>
</gi::Page>
```

### Sample output/test-page
![](https://raw.githubusercontent.com/ProjectGridIron/gridiron/master/github-assets/gridiron-demo.png)
