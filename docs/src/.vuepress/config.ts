import { defineUserConfig } from "vuepress";
import theme from "./theme.js";

export default defineUserConfig({
  base: "/",

  locales: {
    "/": {
      lang: "en-US",
      title: "Verse Engine",
      description: "Verse Engine official documents",
    },
    "/zh/": {
      lang: "zh-CN",
      title: "Verse Engine",
      description: "Verse Engine 官方文档",
    },
  },

  theme,

  shouldPrefetch: false,
});
