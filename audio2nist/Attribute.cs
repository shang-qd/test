using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace audio2nist
{
    [AttributeUsage(AttributeTargets.Property)]
    public class IndexAttribute : Attribute
    {
        /// <summary>
        /// 属性顺序
        /// </summary>
        int _index;
        public IndexAttribute(int index)
        {
            _index = index;
        }
        public int Index
        {
            get { return _index; }
        }
    }

    public class NistInfo
    {
        /// <summary>
        /// 数据类型 
        /// </summary>
        public Type t { get; set; }

        /// <summary>
        /// 数据索引
        /// </summary>
        public int Index { get; set; }
        /// <summary>
        ///  关键字
        /// </summary>
        public string Key { get; set; }
        /// <summary>
        /// 关键字对应的值
        /// </summary>
        public object Val { get; set; }
    }
}
