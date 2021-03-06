/*
 *   Copyright 2003-2004 The Apache Software Foundation.
// (c) Copyright IBM Corp. 2004, 2005 All Rights Reserved
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

/**
 * @author Srinath Perera(hemapani@openource.lk)
 * @author Susantha Kumara(susantha@opensource.lk, skumara@virtusa.com)
 * @author Samisa Abeysinghe (sabeysinghe@virtusa.com)
 */

package org.apache.axis.wsdl.wsdl2ws.cpp;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;

import org.apache.axis.wsdl.wsdl2ws.CUtils;
import org.apache.axis.wsdl.wsdl2ws.SourceWriter;
import org.apache.axis.wsdl.wsdl2ws.WSDL2Ws;
import org.apache.axis.wsdl.wsdl2ws.WrapperFault;
import org.apache.axis.wsdl.wsdl2ws.info.WebServiceContext;

public class ClassLoader implements SourceWriter
{
    private Writer writer;
    private WebServiceContext wscontext;
    private String classname;
    public ClassLoader(WebServiceContext wscontext) throws WrapperFault
    {
        this.wscontext = wscontext;
        this.classname = wscontext.getServiceInfo().getServicename();
    }

    public void writeSource() throws WrapperFault
    {
        String wrappername = classname + CUtils.WRAPPER_NAME_APPENDER;
        try
        {
            this.writer =
                new BufferedWriter(new FileWriter(getFilePath(), false));
            writer.write("/*\n");
			writer.write(" * Copyright 2003-2006 The Apache Software Foundation.\n\n");
			writer.write(" *\n");
			writer.write(" * Licensed under the Apache License, Version 2.0 (the \"License\");\n");
			writer.write(" * you may not use this file except in compliance with the License.\n");
			writer.write(" * You may obtain a copy of the License at\n");
			writer.write(" *\n");
			writer.write(" *\t\thttp://www.apache.org/licenses/LICENSE-2.0\n");
			writer.write(" *\n");
			writer.write(" * Unless required by applicable law or agreed to in writing, software\n");
			writer.write(" * distributed under the License is distributed on an \"AS IS\" BASIS,\n");
			writer.write(" * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n");
			writer.write(" * See the License for the specific language governing permissions and\n");
			writer.write(" * limitations under the License.\n");
			writer.write(" *\n");
            writer.write(" * This file was auto-generated by the Axis C++ Web Service Generator (WSDL2Ws)\n");
            writer.write(" * This file contains the two export functions of the Web service Dynamic Library \n");
            writer.write(" */\n\n");
            writer.write(
                "#include \""
                    + wrappername
                    + CUtils.getHeaderFileExtension()
                    + "\" \n");
            writer.write("extern \"C\" {\n");
            writer.write("STORAGE_CLASS_INFO\n");
            writer.write("int GetClassInstance(BasicHandler **inst)\n");
            writer.write("{\n");
            writer.write("\t*inst = new BasicHandler();\n");
            writer.write(
                "\tWrapperClassHandler* pWCH = new " + wrappername + "();\n");
            writer.write("\t(*inst)->_functions = 0;\n");
            writer.write("\tif (pWCH)\n");
            writer.write("\t{\n");
            writer.write("\t\t(*inst)->_object = pWCH;\n");
            writer.write("\t\treturn pWCH->init();\n");
            writer.write("\t}\n");
            writer.write("\treturn AXIS_FAIL;\n");
            writer.write("}\n");
            writer.write("STORAGE_CLASS_INFO \n");
            writer.write("int DestroyInstance(BasicHandler *inst)\n");
            writer.write("{\n");
            writer.write("\tif (inst)\n");
            writer.write("\t{\n");
            writer.write(
                "\t\tWrapperClassHandler* pWCH = reinterpret_cast<WrapperClassHandler*>(inst->_object);\n");
            writer.write("\t\tpWCH->fini();\n");
            writer.write("\t\tdelete pWCH;\n");
            writer.write("\t\tpWCH = NULL;\n");
            writer.write("\t\tdelete inst;\n");
            writer.write("\t\tinst = NULL;\n");
            writer.write("\t\treturn AXIS_SUCCESS;\n");
            writer.write("\t}\n");
            writer.write("\treturn AXIS_FAIL;\n");
            writer.write("}\n");
            writer.write("}\n");
            writer.flush();
            writer.close();
            if (WSDL2Ws.c_veryVerbose)
                System.out.println(
                    getFilePath().getAbsolutePath() + " created.....");
        }
        catch (IOException e)
        {
            throw new WrapperFault(e);
        }
    }

    protected File getFilePath() throws WrapperFault
    {
        String targetOutputLocation = this.wscontext.getWrapperInfo().getTargetOutputLocation();
        new File(targetOutputLocation).mkdirs();
        String fileName = targetOutputLocation
                + "/" + classname + CUtils.CLASS_LOADER_APPENDER + CUtils.getImplFileExtension();
        this.wscontext.addGeneratedFile(classname + CUtils.CLASS_LOADER_APPENDER + CUtils.getImplFileExtension());
        return new File(fileName);
    }

}
