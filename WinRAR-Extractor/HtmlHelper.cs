using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace WinRAR_Extractor
{
    public class HtmlHelper
    {
        /// <summary>
        /// 使用举例：
        /// 获取关键词 GetSingleTagValueByAttr(data, "meta", "name", "Keywords"); data是HTML源代码
        /// 获取描述 GetSingleTagValueByAttr(data, "meta", "name", "Discription");
        /// 获取跳转 GetSingleTagValueByAttr(data, "meta", "http-equiv", "refresh");
        /// </summary>
        /// <param name="inputstring"></param>
        /// <param name="tagName"></param>
        /// <param name="attrname"></param>
        /// <param name="key"></param>
        /// <returns></returns>
        public static string GetSingleTagValueByAttr(string inputstring, string tagName, string attrname, string key)
        {
            //string reg2 = $"(?<=meta name=\"keywords\" content=\").*?(?=\")";
            string regStr = $"(?<={tagName} {attrname}=\"{key}\" content=\").*?(?=\")";
            //string key_words = Regex.Match(inputstring, regStr).Value;

            //Regex reg = new Regex("<" + tagName + " [^<>]*>", RegexOptions.IgnoreCase);
            Regex reg = new Regex(regStr, RegexOptions.IgnoreCase);
            MatchCollection matchs = reg.Matches(inputstring);
            string result = string.Empty;
            foreach (Match match in matchs)
            {
                string matchValue = match.Value;
                if (!string.IsNullOrEmpty(matchValue))
                {
                    return matchValue;
                }
                //Regex regValue = new Regex("content=".* "", RegexOptions.IgnoreCase);
                //if (matchValue.ToLower().IndexOf(attrname.ToLower() + "="" + key.ToLower() + """) != -1)
                //{
                //    if (regValue.IsMatch(matchValue))
                //    {
                //        result = regValue.Match(matchValue).Value;
                //        if (!string.IsNullOrEmpty(result))
                //        {
                //            //result = result.Replace("CONTENT=", "").Replace("content=", "").Replace(""", "");
                
                //        }
                //    }
                //    return result;
                //}
            }
            return null;
        }
    }
}
