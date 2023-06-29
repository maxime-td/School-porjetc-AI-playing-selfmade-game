this._s=this._s||{};(function(_){var window=this;
try{
_.Zxc=function(a){this.Bk=a};
}catch(e){_._DumpException(e)}
try{
var $xc=function(a){_.on.call(this,a.Ka);var b=this;this.window=a.service.window.get();this.wa=this.Bk();this.oa=window.orientation;this.ka=function(){var c=b.Bk(),d=b.Tdb()&&90===Math.abs(window.orientation)&&b.oa===-1*window.orientation;b.oa=window.orientation;if(c!==b.wa||d){b.wa=c;d=_.Wa(b.Ge);for(var e=d.next();!e.done;e=d.next()){e=e.value;var f=new _.Zxc(c);try{e(f)}catch(g){_.ca(g)}}}};this.Ge=new Set;this.window.addEventListener("resize",this.ka);this.Tdb()&&this.window.addEventListener("orientationchange",
this.ka)};_.A($xc,_.on);$xc.wb=_.on.wb;$xc.Fa=function(){return{service:{window:_.pn}}};$xc.prototype.addListener=function(a){this.Ge.add(a)};$xc.prototype.removeListener=function(a){this.Ge.delete(a)};
$xc.prototype.Bk=function(){if(ayc()){var a=_.ll(this.window);a=new _.Lk(a.width,Math.round(a.width*this.window.innerHeight/this.window.innerWidth))}else a=this.Wb()||(_.ta()?ayc():this.window.visualViewport)?_.ll(this.window):new _.Lk(this.window.innerWidth,this.window.innerHeight);return a.height<a.width};$xc.prototype.destroy=function(){this.window.removeEventListener("resize",this.ka);this.window.removeEventListener("orientationchange",this.ka)};var ayc=function(){return _.ta()&&_.Lg.lE()&&!navigator.userAgent.includes("GSA")};
$xc.prototype.Wb=function(){return _.byc};$xc.prototype.Tdb=function(){return"orientation"in window};_.byc=!1;_.qn(_.AZa,$xc);
_.byc=!0;
}catch(e){_._DumpException(e)}
try{
_.m("aLUfP");

_.n();
}catch(e){_._DumpException(e)}
})(this._s);
// Google Inc.
