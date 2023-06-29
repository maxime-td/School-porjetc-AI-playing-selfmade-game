this._s=this._s||{};(function(_){var window=this;
try{
_.UVe=_.I("Lhx8ef");
}catch(e){_._DumpException(e)}
try{
_.m("w4UyN");
var jws=function(a){_.y.call(this,a.Ka);this.ka=!1;this.oa=_.Mc("elPddd");this.rootElement=this.getRoot().el()};_.A(jws,_.y);jws.Fa=_.y.Fa;jws.prototype.wa=function(){if(""===_.w.getStyle(this.oa,"transform")){if(_.pv(this.rootElement),_.Te(document,_.UVe),!this.ka){var a=_.AD(new _.yD,_.zD(new _.xD,134634));_.Te(document,_.BD,{iI:a});this.ka=!0}}else _.w.setStyle(this.oa,"transform","");this.hb("suEOdc").setStyle("visibility","hidden")};
jws.prototype.showTooltip=function(){this.hb("suEOdc").setStyle("visibility","inherit")};jws.prototype.Qj=function(){this.hb("suEOdc").setStyle("visibility","hidden")};_.K(jws.prototype,"LfDNce",function(){return this.Qj});_.K(jws.prototype,"eGiyHb",function(){return this.showTooltip});_.K(jws.prototype,"HfCvm",function(){return this.wa});_.Q(_.E5a,jws);
_.n();
}catch(e){_._DumpException(e)}
try{
_.RNi=function(a,b){var c="h";c=void 0===c?"m":c;var d=void 0===d?!0:d;var e=void 0===e?a:e;if(_.Tc("l")){var f=window.localStorage;e=new _.Td("l",e);b=_.Wa(b);for(var g=b.next();!g.done;g=b.next()){g=g.value;var h=a+"::"+g,l=f.getItem(h);d&&f.removeItem(h);null===e.get(g)&&null!==l&&(h=JSON.parse(l),null!==h&&e.set(g,h,c))}}};
}catch(e){_._DumpException(e)}
try{
_.m("Exk9Ld");
var VDs;_.UDs={name:"abar"};VDs=function(a){_.y.call(this,a.Ka);_.RNi(_.UDs.name,["bbh"]);a=_.Sd("l",_.UDs);var b=1===a.get("bbh");_.Bf(this.getData("showSafesearchGlobalNotice"),!1)?a.set("bbh",1,"h"):b||(this.getRoot().show(),a.set("bbh",1,"h"))};_.A(VDs,_.y);VDs.Fa=_.y.Fa;VDs.prototype.ka=function(){this.getRoot().hide()};_.K(VDs.prototype,"R194mf",function(){return this.ka});_.Q(_.Zgb,VDs);
_.n();
}catch(e){_._DumpException(e)}
try{
_.m("i9SNBf");
var WFh=_.I("dXIA6");var XFh=function(a){_.y.call(this,a.Ka);this.rootElement=this.getRoot().el();this.checkbox=_.Do(this,"MPu53c").el();if(a=_.Wn(this.rootElement,"labelledby")){var b=document.getElementById(a);b&&(b.setAttribute("for",this.checkbox.getAttribute("id")),_.Un(this.checkbox,"labelledby",a))}};_.A(XFh,_.y);XFh.Fa=_.y.Fa;XFh.prototype.setChecked=function(a,b){this.checkbox.checked!==a&&(this.checkbox.checked=a,(void 0===b||b)&&this.trigger(WFh))};_.Q(_.U8a,XFh);
_.n();
}catch(e){_._DumpException(e)}
try{
_.m("UBXHI");
_.F3e=function(a){_.y.call(this,a.Ka);this.loaded=!1;this.root=this.getRoot()};_.A(_.F3e,_.y);_.F3e.Fa=_.y.Fa;_.F3e.prototype.ka=function(){if(!this.loaded){var a=_.km(this.root.getData("src"));a&&(this.root.zc("src",a),this.root.setData("src",null));this.loaded=!0}};_.K(_.F3e.prototype,"QT8fkf",function(){return this.ka});_.Q(_.r7a,_.F3e);
_.n();
}catch(e){_._DumpException(e)}
try{
_.m("TSZEqd");
_.fC=function(a){_.y.call(this,a.Ka);this.loaded=!1;this.window=a.service.window;this.image=this.getRoot();this.ka()};_.A(_.fC,_.y);_.fC.Fa=function(){return{service:{window:_.pn}}};
_.fC.prototype.ka=function(){var a;if(a=!this.loaded)a=_.Pic(this.image.el()),a=0===a.offsetHeight&&0===a.offsetWidth||"hidden"===_.w.getComputedStyle(a,"visibility")||!_.w.NJ(a)?!1:(a=a.parentElement.getBoundingClientRect())?a.left>this.window.get().scrollX+this.window.get().innerWidth||a.left+a.width<this.window.get().scrollX?!1:!0:!0;a&&this.load()};
_.fC.prototype.load=function(){if(!this.loaded){var a=this.image.getData("src");a.Ib()&&(this.image.zc("src",a.string()),this.image.setData("src",null));this.loaded=!0}};_.fC.prototype.oa=function(){var a=this.image.getData("src");a.Ib()&&a.string()!==this.image.Pc("src")||this.image.parent().setStyle("background-color","")};_.K(_.fC.prototype,"K1e2pe",function(){return this.oa});_.K(_.fC.prototype,"eNYuKb",function(){return this.load});_.K(_.fC.prototype,"KpWyKc",function(){return this.ka});
_.Q(_.A8a,_.fC);
_.n();
}catch(e){_._DumpException(e)}
try{
_.m("R3fhkb");
var G3e=function(a){_.F3e.call(this,a.Ka);this.window=a.service.window;this.oa()};_.A(G3e,_.F3e);G3e.Fa=function(){return{service:{window:_.pn}}};
G3e.prototype.oa=function(){if(!this.loaded){var a=this.root.Eb();if((0!==a.offsetHeight||0!==a.offsetWidth)&&"hidden"!==_.w.getComputedStyle(a,"visibility")&&_.w.NJ(a.parentElement)){var b;(a=null==(b=a.parentElement)?void 0:b.getBoundingClientRect())&&(a.left>this.window.get().scrollX+this.window.get().innerWidth||a.left+a.width<this.window.get().scrollX)||this.ka()}}};_.K(G3e.prototype,"iSvHDf",function(){return this.oa});_.Q(_.s7a,G3e);
_.n();
}catch(e){_._DumpException(e)}
try{
_.m("n8YO7e");
var Pjg=function(a){_.y.call(this,a.Ka);var b=this;this.closed=!1;this.Rj=a.ff.Rj;this.Ub=a.service.Ub;window.IntersectionObserver&&(this.oa=new IntersectionObserver(function(c){c[0].isIntersecting?b.ka=_.ve(document.body,"click",b.mtd,b):b.ka&&_.ue(b.ka)},{threshold:1}));this.Da("EFexIf").Xd()&&this.init()};_.A(Pjg,_.y);Pjg.Fa=function(){return{service:{Ub:_.Yt},ff:{Rj:"SgxdIe"}}};_.k=Pjg.prototype;_.k.init=function(){_.Rd(this.Da("bN97Pc").el(),"click",this.Dic,void 0,this);this.oa&&this.oa.observe(this.getRoot().el())};
_.k.Kb=function(){this.Pe();_.y.prototype.Kb.call(this)};_.k.Pe=function(){this.oa&&this.oa.disconnect();this.ka&&_.ue(this.ka);_.sk(this.Da("bN97Pc").el(),"click",this.Dic,void 0,this)};_.k.Dic=function(a){var b;a.target&&(b=_.le(a.target,function(c){return _.ke(c)&&!!_.uc(c,"ved")},!0));this.close(b?b:void 0)};_.k.ntd=function(){this.closed||this.close(this.Da("EFexIf").el());return!0};_.k.mtd=function(a){(a=a.targetElement&&a.targetElement.el())&&Qjg(this,a)||this.close();return!1};
var Qjg=function(a,b){return!!_.le(b,function(c){return _.ke(c)&&c.getAttribute("jscontroller")===a.getRoot().Pc("jscontroller")},!0)};
Pjg.prototype.close=function(a){this.closed=!0;var b=this.Da("EFexIf");_.Ye(this.getRoot().el(),_.Pl(document))&&(_.Kic(this.Da("bN97Pc").el(),function(d){return _.Ll(d)})||this.getRoot().Eb()).focus();this.Da("tqp7ud").el().setAttribute("disabled","true");b.el().setAttribute("aria-hidden","true");var c=_.Qx();c.add((new _.Jx(b.el(),{duration:100})).Rd(.001));c.build().play().then(function(){b.hide()});this.Pe();this.Rj&&this.Rj.recordDismissal();c=this.Ub.ka();a&&(c=c.ka(a));_.cxc(c,b.el()).log()};
Pjg.prototype.show=function(){this.Da("EFexIf").show();this.init()};_.K(Pjg.prototype,"ti6hGc",function(){return this.show});_.K(Pjg.prototype,"I9owB",function(){return this.ntd});_.K(Pjg.prototype,"k4Iseb",function(){return this.Kb});_.Q(_.C7a,Pjg);

_.n();
}catch(e){_._DumpException(e)}
})(this._s);
// Google Inc.
