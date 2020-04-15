package com.nelson.player.utils;

public class CommonUtil {
    public static String getTime(long second) {
        long hours = second / 3600;//转换小时数
        second = second % 3600;//剩余秒数
        long minutes = second / 60;//转换分钟
        second = second % 60;//剩余秒数
        if (hours > 0) {
            return unitFormat(hours) + ":" + unitFormat(minutes) + ":" + unitFormat(second);
        } else {
            return unitFormat(minutes) + ":" + unitFormat(second);
        }
    }

    private static String unitFormat(long i) {
        String retStr;
        if (i >= 0 && i < 10) {
            retStr = "0" + i;
        } else {
            retStr = "" + i;
        }
        return retStr;
    }
}
