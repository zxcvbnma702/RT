# RayTracing

## What is RayTracing

> 光线追踪（Ray Tracing）是一种图形渲染技术，用于生成高度逼真的图像。光线追踪模拟光线在场景中的传播和交互，以实现逼真的光照效果、阴影、反射、折射和全局光照。
>
> 光线追踪基于光的物理行为，通过追踪从视点出发的光线，与场景中的物体交互来模拟视觉效果


光线追踪算法是一种非常自然的技术，相比于光栅化的方法，它更加简单、暴力、真实。与光栅化根据物体计算所在的像素的方式不同，光线路径追踪的方法是一个相反的过程，它在于用眼睛去看世界而不是世界如何到达眼中。如下图所示，从视点出发向屏幕上每一个像素发出一条光线View Ray，追踪此光路并计算其逆向光线的方向，映射到对应的像素上。通过计算光路上颜色衰减和叠加，即可基本确定每一个像素的颜色。

![alt text](image-71.png)

[reference](www.youtube.com/c/QuantitativeBytes)

## 窗口

使用 SDL2 创建窗口

> SDL（Simple DirectMedia Layer）是一个跨平台的多媒体开发库，用于访问低级硬件（如图形、声音和输入设备）。

## Camera （摄像机）

![alt text](image.png)

![alt text](image-1.png)

![alt text](image-2.png)

**摄像机位置（Camera Position）**：摄像机在世界坐标系中的位置，记为 $\mathbf{Corg}$。
**看向的位置（Look At Position）**：摄像机所望向的点，记为 $\mathbf{LookAt}$。
**上方向向量（Up Vector）**：定义摄像机的上方向，记为 $\mathbf{Up}$。
**焦距（Focal Length）**：从摄像机到投影平面的距离，记为 $f$。
**投影平面的尺寸**：水平尺寸 $W$ 和垂直尺寸 $H$。
**宽高比（Aspect Ratio）**：投影平面宽度与高度的比值，记为 $\text{AR}$。
例如，宽高比为 16:9，则 $\text{AR} = 16/9$。

![alt text](image-3.png)

> 向量计算
> 
1. **对齐向量（Forward Vector）**：从摄像机位置指向看向点的单位向量，表示摄像机的朝向。
$$
\mathbf{F} = \frac{\mathbf{LookAt} - \mathbf{Corg}}{|\mathbf{LookAt} - \mathbf{Corg}|}
$$

2. **水平向量（Right Vector）**：摄像机水平方向，由对齐向量和上方向向量的叉积得到。
$$
\mathbf{U} = \frac{\mathbf{F} \times \mathbf{Up}}{|\mathbf{F} \times \mathbf{Up}|}
$$

3. **直向量（Up Vector）**：由水平向量和对齐向量的叉积得到，重新计算后的上方向向量。
$$
\mathbf{V} = \mathbf{U} \times \mathbf{F}
$$

> 平面计算

1. **投影平面中心点**：从摄像机位置沿对齐向量方向前进焦距的点，表示投影平面的中心。
$$
\mathbf{P}_\text{center} = \mathbf{Corg} + f \mathbf{F}
$$

2. **投影平面的尺寸调整**：
$$
\mathbf{U}_\text{plane} = \mathbf{U} \times W
$$
$$
\mathbf{V}_\text{plane} = \mathbf{V} \times H
$$

todo 画图

> 图像平面（Image Plane）是指在计算机图形学和计算机视觉领域中，虚拟摄像机在空间中生成图像的那个平面。它是场景中从三维到二维投影的平面，简单来说，就是摄像机所“看到”的东西最终映射到这个平面上形成的图像。

### 计算实例

- m_CameraPosition: 摄像机在世界坐标系中的位置，假设为 (0, 0, 0)
- m_CameraLookAt: 摄像机看向的目标点，假设为 (0, 0, -1)
- m_CameraUp: 摄像机的上方向向量，假设为 (0, 1, 0)
- m_CameraLength: 摄像机到图像平面的距离，假设为 1
- m_CameraHorizontalSize: 图像平面的水平尺寸，假设为 2
- m_CameraAspectRatio: 图像平面的长宽比，假设为 1 （意味着图像平面的垂直尺寸也为 2

```c++
// 图形平面计算
// 1. 计算摄像机朝向向量：
m_AlimentVector = m_CameraLookAt - m_CameraPosition 
                = (0, 0, -1) - (0, 0, 0) 
                = (0, 0, -1)
m_AlimentVector.Normalize() // 归一化后的向量仍为 (0, 0, -1)

// 2. 计算图像平面的 U 向量和 V 向量：
m_ProjectScreenU = cross(m_AlimentVector, m_CameraUp) 
                 = cross((0, 0, -1), (0, 1, 0)) 
                 = (1, 0, 0)
m_ProjectScreenU.Normalize() // 归一化后的向量仍为 (1, 0, 0)
m_ProjectScreenV = cross(m_ProjectScreenU, m_AlimentVector) 
                 = cross((1, 0, 0), (0, 0, -1)) 
                 = (0, 1, 0)
m_ProjectScreenV.Normalize() // 归一化后的向量仍为 (0, 1, 0)

// 3. 扩展向量的长度
m_ProjectScreenU = m_ProjectScreenU * m_CameraHorizontalSize 
                 = (1, 0, 0) * 2 
                 = (2, 0, 0)
m_ProjectScreenV = m_ProjectScreenV * (m_CameraHorizontalSize / m_CameraAspectRatio) 
                 = (0, 1, 0) * 2 
                 = (0, 2, 0)

// 4. 计算图像平面的中心点：
m_ProjectScreenCenter = m_CameraPosition + (m_CameraLength * m_AlimentVector) 
                      = (0, 0, 0) + (1 * (0, 0, -1)) 
                      = (0, 0, -1)
```
```c++
// 生成射线示例
// 假设要生成图像平面上归一化坐标为 (proScreenX, proScreenY) = (0.5, 0.5) 对应的射线。

// 1. 计算图像平面上的世界坐标：
screenWorldPart1 = m_ProjectScreenCenter + (m_ProjectScreenU * proScreenX)
                 = (0, 0, -1) + (2, 0, 0) * 0.5
                 = (0, 0, -1) + (1, 0, 0)
                 = (1, 0, -1)
screenWorldCoordinate = screenWorldPart1 + (m_ProjectScreenV * proScreenY)
                       = (1, 0, -1) + (0, 2, 0) * 0.5
                       = (1, 0, -1) + (0, 1, 0)
                       = (1, 1, -1)
// 2. 生成射线
RT::Ray(m_CameraPosition, screenWorldCoordinate)

// 该射线的起点为 (0, 0, 0)，
// 方向为从摄像机位置 (0, 0, 0) 指向图像平面上的坐标 (1, 1, -1) 的向量。
```

## Ray - Sphere Intersections（光线-球体🍌）

![alt text](image-5.png)

![alt text](image-4.png)

![alt text](image-6.png)

![alt text](image-7.png)

## Point Light （点光源）

![alt text](image-8.png)

![alt text](image-9.png)

## GETR

![alt text](image-10.png)

> 单位矩阵

![alt text](image-11.png)

> 缩放变换

![alt text](image-13.png)

> 旋转变换

![alt text](image-12.png)

![alt text](image-14.png)

![alt text](image-15.png)

![alt text](image-16.png)

![alt text](image-17.png)

![alt text](image-18.png)

> 平移变换

![alt text](image-19.png)

![alt text](image-20.png)

![alt text](image-21.png)

![alt text](image-22.png)

![alt text](image-23.png)

目前定义的球体

![alt text](image-24.png)

有了变换就可以将球体的物理坐标转换为世界坐标，
> 在3D图形学中，通常我们有一个物体的局部坐标系（例如球体的物理坐标），然后通过一系列变换（平移、旋转、缩放等），将该物体从局部坐标系转换到世界坐标系中。


## Plane & Shadows （平面与阴影）

![alt text](image-25.png)

![alt text](image-26.png)

![alt text](image-27.png)

![alt text](image-28.png)

![alt text](image-29.png)

![alt text](image-30.png)

![alt text](image-31.png)

![alt text](image-32.png)

### shadow

1. 初次光线投射（Primary Ray Casting）:
首先，从相机位置发射初始光线。每条光线穿过屏幕上的一个像素，并与场景中的物体进行相交测试，找到最近的交点。这一步用来判断光线击中了什么物体。

2. 阴影光线投射（Shadow Ray Casting）:
当确定初始光线与某个物体相交后，需要计算该点是否处于阴影中。为此，从交点向场景中的每个光源发射阴影光线。

3. 计算阴影光线: 从交点（Intersection Point）向光源发出一条光线（Shadow Ray）。
检测遮挡物: 检查阴影光线在其路径上是否与其他物体相交。如果有物体阻挡了这条光线，则交点处于阴影之中，意味着该点不能直接受到光照。
光照计算:

> 如果阴影光线到达光源时没有遇到任何遮挡物，则该点直接受到光照，可以继续计算光照贡献（包括直接光照和反射光照）。
如果阴影光线被遮挡，则认为该点处于阴影中，来自该光源的光不会对该点产生直接光照。

## Material（布林冯模型）

![alt text](image-33.png)

布林冯反射模型

> 将光分为3个部分，分别是环境光照(ambient lighting)，漫反射光照(diffuse reflection，高光(specular highlights)

![alt text](image-39.png)

[games101](https://www.bilibili.com/video/BV1X7411F744/?p=7&vd_source=b3b87210888ec87be647603921054a36)

- 觀測向量 v
- 表面法線 n
- 光照方向 l

> 漫反射分量

$$
    L_d = K_d \frac{E}{r^2} \max(0, \mathbf{\hat{n}} \cdot \mathbf{\hat{l}})
$$

$L_d$（扩散反射光强度，Diffuse Light Intensity）：这是物体接收到并反射出的光线的强度，对于每个表面点来讲，它是通过漫反射计算出来的光线强度。

$K_d$（漫反射係數嗎，Diffuse Reflectance Coefficient）：光到达物体表面后，能量会被吸收一部分（不同材质的物体对不同波长的光线吸收率是不一样的），剩下的会被反射出来，漫反射系数即定义了不被物体所吸收的光照颜色`
diffuseColor`，即物体表现出的颜色。

$\frac{E}{r^2}$(光照能量衰減係數)：光照能量衰减系数，抵达物体表面的能量与光源和着色点距离平方成反比。

$\max(0, \mathbf{\hat{n}} \cdot \mathbf{\hat{l}})$：表面着色点接收到的能量正比于$\mathbf{\hat{n}} \cdot \mathbf{\hat{l}}$。为了避免负值情况使用max做了最小值的限定

![alt text](image-40.png)

![alt text](image-35.png)

> 高光

$$
L_s = K_s \frac{E}{r^2} \max(0, \cos \theta)^p = K_s \frac{E}{r^2} \max(0, \mathbf{\hat{n}} \cdot \mathbf{\hat{h}})^p
$$

$L_s$ ：镜面反射光强度 (Specular Light Intensity)。这是物体表面由于镜面反射作用反射的光强度。

$K_s$ ：镜面反射系数 (Specular Reflectance Coefficient)。这是物体材质属性，表示物体表面对镜面反射光的反射能力。值越高，镜面反射效果越强。

$E$ ：光源光强 (Light Intensity)。这是来自光源的光的强度。

$r$ ：光源到表面点的距离 (Distance from Light Source to Surface Point)。光源与物体表面点之间的直线距离。公式中用 $r^2$ 是为了计算距离引起的光衰减。

$\cos \theta$ ：光源方向与镜面反射方向之间的余弦值。这是光源方向和镜面反射方向向量之间夹角的余弦。对于镜面反射，镜面反射方向 (R) 通常是视角方向的反射。

$\mathbf{\hat{n}} \cdot \mathbf{\hat{h}}$ ：单位表面法线向量 $\mathbf{\hat{n}}$ 与半程向量 $\mathbf{\hat{h}}$ 的点积。

$\mathbf{\hat{n}}$：表面法线向量 (Normal Vector)。这是垂直于表面的单位向量，表示表面方向。
$\mathbf{\hat{h}}$：半程向量 (Halfway Vector)。这是光源方向向量 $\mathbf{\hat{l}}$ 和观察方向向量 $\mathbf{\hat{v}}$ 的归一化和。公式为：
$$
\mathbf{\hat{h}} = \frac{\mathbf{\hat{l}} + \mathbf{\hat{v}}}{|\mathbf{\hat{l}} + \mathbf{\hat{v}}|}
$$
$p$ ：高光指数 (Shininess Coefficient)，也称为高光锐度指数或镜面反射指数 (Specular Exponent)。这个参数控制高光的锐利程度。值越大，镜面反射的高光区域越小越尖锐，值越小，高光区域越大越柔和。

$\max(0, \cos \theta)$ 和 $\max(0, \mathbf{\hat{n}} \cdot \mathbf{\hat{h}})$：这些函数确保了只有当光线方向与法线方向和半程向量方向成锐角时才有正的贡献，因为负值（光线在表面后方）不产生反射强度。

![alt text](image-34.png)

> 环境光

布林冯模型假设环境光是一个常量

$$
 L_a = K_a E_a
$$

![alt text](image-36.png)

> 总结

将三种光照作用效果叠加在一起，就可以得到近似的物理光照效果

$$
I \\ = I_{\text{ambient}} + I_{\text{diffuse}} + I_{\text{specular}} \\ = K_a E_a + K_d \frac{E}{r^2} \max(0, \mathbf{\hat{n}} \cdot \mathbf{\hat{l}}) + K_s \frac{E}{r^2} \max(0, \mathbf{\hat{n}} \cdot \mathbf{\hat{h}})^p
$$

> 反射

$$
\mathbf{R} = \mathbf{V} - 2 (\mathbf{V} \cdot \mathbf{N}) \mathbf{N}
$$

$\mathbf{V}$ 是入射光线的方向向量，应该是一个单位向量。
$\mathbf{N}$ 是反射面的法线向量，也是一个单位向量，通常指向介质外部。
$\mathbf{V} \cdot \mathbf{N}$ 表示向量的点积运算。
$\mathbf{R}$ 是反射光线的方向向量，也是一个单位向量。

![alt text](image-37.png)

![alt text](image-38.png)

## Cone and Cylinder

![alt text](image-41.png)

![alt text](image-42.png)

![alt text](image-43.png)

![alt text](image-44.png)

![alt text](image-45.png)

![alt text](image-46.png)

得到交点方程

$$
(p_x + t v_x)^2 + (p_y + t v_y)^2 = r^2
$$

展开公式

$$
t^2 v_x^2 + 2 p_x v_x t + p_x^2 + t^2 v_y^2 + 2 p_y v_y t + p_y^2 = r^2
$$

合并同类项

$$
t^2 v_x^2 + t^2 v_y^2 + 2 p_x v_x t + 2 p_y v_y t + p_x^2 + p_y^2 = r^2
$$

提出参数

$$
t^2 (v_x^2 + v_y^2) + 2t (p_x v_x + p_y v_y) + (p_x^2 + p_y^2) = r^2
$$

根据 

$$
at^2 + bt + c = 0
$$

推导出

$$ a = v_x^2 + v_y^2 $$ $$ b = 2(p_xv_x + p_yv_y) $$ $$c=p_x^2 + p_y^2 -r^2 $$

根据求根公式

$$ t = \frac{-B \pm \sqrt{B^2 - 4AC}}{2A} $$

![alt text](image-47.png)

![alt text](image-48.png)

![alt text](image-49.png)

![alt text](image-50.png)

![alt text](image-51.png)

![alt text](image-52.png)

## UV Space & Textures (uv展开 & 纹理)

> UV Unwrapping 是将 3D 模型的表面展开成一个平坦的二维表示，使得每一个3D表面都能准确地对应到2D纹理图像上的特定部分。
>
> 笛卡尔坐标系 -> 柱面坐标系
>
> [referece](https://zhuanlan.zhihu.com/p/369977849)

![alt text](image-54.png)

![alt text](image-53.png)

![alt text](image-55.png)

![alt text](image-56.png)

![alt text](image-57.png)

![alt text](image-58.png)

![alt text](image-59.png)

![alt text](image-60.png)

> 球面映射
>   
> [reference][referenceLink2]

因为我们假定半径r为1，那么对于球面坐标系而言表示一个方向只需要两个变量，一个变量是极角 $\theta$ 相当于纬度，另一个是方位角 $\varphi$ 相当于经度，这两个变量的作用域分别是 $0 ≤θ≤ π$ 和 $ 0 ≤φ< 2π $，我们需要将其映射到uv坐标系的 $（0,1)$ 范围内。

![alt text](image-61.png)

1. 对于已知点 $(x, y, z)$ , 将其转换为球坐标系 $(r,θ,ϕ)$。
$$
\begin{cases}
r = \sqrt{x^2 + y^2 + z^2} \\
\theta = \arccos \left( \frac{z}{r} \right)  = \arcsin\left(\frac{\sqrt{x^2 + y^2}}{r}\right) = \arctan\left(\frac{\sqrt{x^2 + y^2}}{z}\right) \\
\varphi = \arccos\left(\frac{x}{r \sin \theta}\right) = \arcsin\left(\frac{y}{r \sin \theta}\right) = \arctan\left(\frac{y}{x}\right)
\end{cases}
$$

1. 参数化修正

实际应用中，UV坐标通常需要归一化到[0, 1]或[-1, 1]，因此：

$$
\begin{cases}
u = \frac{\varphi}{2\pi} = \frac{1}{2\pi} \arctan \left( \frac{y}{x} \right) \\
v = \frac{\theta}{\pi} = \frac{1}{\pi} \arccos \left( \frac{z}{r} \right)
\end{cases}
$$

3. 公式等价变形

等价变形后的公式为

$$
\begin{cases}
u = \arctan \left( \frac{\sqrt{x^2 + y^2}}{z} \right) \\
v = \arctan \left( \frac{y}{x} \right)
\end{cases}
$$

4. 归一化处理

$$
\begin{cases}
u_{\text{norm}} = 2 \cdot \dfrac{u}{\pi} - 1 \\
v_{\text{norm}} = 2 \cdot \dfrac{v}{\pi} - 1
\end{cases}
$$

![alt text](image-63.png)

![alt text](image-64.png)

![alt text](image-65.png)

![alt text](image-66.png)

![alt text](image-67.png)

### 图片 load

使用 sdl_image 库，将图片像素由 uv 转化为 xy 范围

> 统一调整范围

u 和 v 的范围在 [-1, 1] 之间，我们需要将其调整到 [0, 1] 范围内。因此采取以下步骤：

$$ u' = \frac{u + 1}{2} $$
$$ v' = \frac{v + 1}{2} $$

> 转换为像素坐标

将调整后的范围 [0, 1] 转换为图像宽度和高度范围 [0, m_xSize] 和 [0, m_ySize]。

$$ x = \left\lfloor u' \times m_xSize \right\rfloor $$
$$ y = \left\lfloor v' \times m_ySize \right\rfloor $$

![alt text](image-68.png)

[referenceLink2]: https://zhuanlan.zhihu.com/p/594429859

## Refractive Materials（折射材料）

> 折射材料（Refractive Materials）用来模拟现实世界中透明或半透明材料对光的折射行为。这类材料可以真实地再现如玻璃、水、宝石等在光线经过时产生的物理效果，使得渲染的图像更加逼真。
>
> 当光线从一种介质进入另一种介质时，光线的方向改变。这个变化可以用斯涅尔定律（Snell's Law）来描述。

斯涅尔定律：$$n_1 \sin \theta_1 = n_2 \sin \theta_2$$

![alt text](image-69.png)

![alt text](image-70.png)

1. 斯涅尔定律的基本形式
斯涅尔定律在标量形式下表示为：
$$
n_1 \sin \theta_1 = n_2 \sin \theta_2
$$
其中，$n_1$ 和 $n_2$ 是两种介质的折射率，$\theta_1$ 是入射角，$ \theta_2$ 是折射角。

1. 矢量的定义
设入射方向的单位矢量为 $\vec{v_{incident}}$，折射方向的单位矢量为 $\vec{v_{refract}}$，法向量为 $\vec{n}$。

1. 使用法向量表示入射和折射角的余弦
我们有以下关系：
$$
\cos \theta_1 = - \vec{n} \cdot \vec{v_{incident}}
$$
$$
\cos \theta_2 = - \vec{n} \cdot \vec{v_{refract}}
$$

1. 矢量形式的几何关系
将入射矢量分解为平行法向量和垂直法向量的部分:
$$
\vec{v_{incident}} = \vec{v}{\parallel} + \vec{v}{\perp}
$$
这里，$\vec{v}{\parallel}$ 是平行于法向量的部分，$\vec{v}{\perp}$ 是垂直于法向量的部分:
$$
\vec{v}{\parallel} = -(\vec{n} \cdot \vec{v_{incident}}) \vec{n} = -\cos \theta_1 \vec{n}
$$
$$
\vec{v}{\perp} = \vec{v_{incident}} - \vec{v}{\parallel} = \vec{v_{incident}} + \cos \theta_1 \vec{n}
$$

对于折射矢量，我们有类似的分解:
$$
\vec{v_{refract}} = \vec{v}'{\parallel} + \vec{v}'_{\perp}
$$

5. 利用斯涅尔定律和几何关系
根据斯涅尔定律：
$$
n_1 \sin \theta_1 = n_2 \sin \theta_2 \quad \Rightarrow \quad \sin \theta_2 = \frac{n_1}{n_2} \sin \theta_1
$$

垂直于法线的分量不变：
$$
\vec{v}'{\perp} = \frac{n_1}{n_2} \vec{v}{\perp}
$$

平行于法线的分量通过折射角的余弦变化：
$$
\cos \theta_2 = \sqrt{1 - \left( \frac{n_1}{n_2} \sin \theta_1 \right)^2}
$$

6. 总的折射矢量表示
折射矢量的总形式：
$$
\vec{v_{refract}} = \frac{n_1}{n_2} (\vec{v_{incident}} + \cos \theta_1 \vec{n}) - \cos \theta_2 \vec{n}
$$

将上面的分解代入并整理得到：
$$
\vec{v_{refract}} = \left( \frac{n_1}{n_2} \right) \vec{v_{incident}} + \left( \left( \frac{n_1}{n_2} \cos \theta_1 \right) - \cos \theta_2 \right) \vec{n}
$$

最终表达式
使用 $r = \frac{n_1}{n_2}$ 和 $c = \cos \theta_1 = - \vec{n} \cdot \vec{v_{incident}} $，并结合我们对 $\cos \theta_2$ 的计算：
$$
\vec{v_{refract}} = r \vec{v_{incident}} + \left( r c - \sqrt{1 - r^2(1 - c^2)} \right) \vec{n}
$$

这就是斯涅尔定律的矢量形式的推导过程。


![alt text](image-72.png)
